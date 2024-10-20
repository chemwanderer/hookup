#pragma once
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// 0     | 0x55 - header byte                                                //
// 1     | 0xnn - device ID                                                  //
// 2     | 0xnn - package size                                               //
// 3     | 0xnn - N-sized package }                                          //
// ...                            }                                          //
// N + 2 | 0xnn - N-sized package }                                          //
// N + 3 | 0xnn - checksum                                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include "engine/routine/engine_routine_base.h"
#include "engine/routine/mbed/device/engine_routine_mbed_device_base.h"
#include <QSerialPort>
#include <deque>

namespace engine::routine {
	class Base;

	template<RoutineBox T>
	class Factory;
}

namespace engine::routine::mbed {
	class Server :
		public Base
	{
		friend class Factory<std::vector<Base*>>;

	private:
		static constexpr char HEADER_BYTE = 0x55;
		static constexpr quint32 
			INVALID_IDX = 0xff'ff'ff'ff,

			SERIAL_OPEN_ATTEMPT_PAUSE = 500,	// ms
			READ_ATTEMPT_DURATION = 10,			// ms
			MAX_READ_ATTEMPTS_NUMBER = 100,

			WRAPPER_SIZE = 4,
			HEADER_BYTE_POSITION = 0,
			DEVICE_ID_POSITION = 1,
			PACKAGE_SIZE_POSITION = 2,
			PACKAGE_HEAD_POSITION = 3,

			BAUD_RATE = 9600;

	public:
		virtual ~Server() noexcept override final;
		Server(const Server&) = delete;

	private:
		explicit Server(const QString& _uniqueName, const QString _togglePrmID, const QString& _currentStatusPrmID) noexcept;

	private:
		virtual void run() override final;
		virtual void beforeQuit() override final;

	private:
		quint32 currentDeviceIdx() const;
		device::Base* currentDevice();
		void nextDevice();

		void setupSerialPort();
		bool readResponse(quint32 _deviceIdx);
		bool processResponse();
		void processAllResponses();
		device::Base* device(uchar _id);

	private:
		QSerialPort* pSerialPort_{ nullptr };
		QString portName_{};

		quint32 currentDeviceIdx_{ INVALID_IDX };
		std::vector<device::Base*> devices_{};
		std::deque<std::tuple<quint32, uchar, QByteArray>> responses_{};

	private:
		static QByteArray wrap(const QByteArray& _package, uchar _deviceID);
		static QByteArray unwrap(const QByteArray& _message, uchar* _pDeviceID, bool* _pOK);
		static void writeChecksum(char* _pDst, quint32 _size);
		static bool verifyChecksum(const char* _pSrc, quint32 _size);
	};

	inline Server::Server(const QString& _uniqueName, const QString _togglePrmID, const QString& _currentStatusPrmID) noexcept :
		Base(_uniqueName, _togglePrmID, _currentStatusPrmID)
	{}

	inline quint32 Server::currentDeviceIdx() const {
		return currentDeviceIdx_;
	}

	inline device::Base* Server::currentDevice() {
		return devices_[currentDeviceIdx_];
	}

	inline void Server::nextDevice() {
		++currentDeviceIdx_;
		if (devices_.size() <= currentDeviceIdx_) { currentDeviceIdx_ = 0; }
	}
}
