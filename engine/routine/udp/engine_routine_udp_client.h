#pragma once
#include "engine/routine/engine_routine_base.h"
#include <QUdpSocket>

namespace engine::routine {
	class Base;

	template<RoutineBox T>
	class Factory;
}

namespace engine::routine::udp {
	class Client :
		public Base
	{
		friend class Factory<std::vector<Base*>>;

	private:
		static constexpr quint32 MAX_ATTEMPTS = 200;
		static constexpr qint32 ATTEMPT_DURATION = 10;

	public:
		virtual ~Client() noexcept override final = default;
		Client(const Client&) = delete;

	private:
		explicit Client(const QString& _uniqueName, 
			const QString _togglePrmID, const QString& _currentStatusPrmID) noexcept;

	private:
		virtual void run() override final;
		virtual void beforeQuit() override final;

	private:
		void iteration();

		void sendReadRequest();
		void sendWriteRequest();
		bool getResponse();

		IValue* current();
		void next();
		void updateCurrent();

	private:
		QUdpSocket* pReadingSocket_{ nullptr };
		QUdpSocket* pSendingSocket_{ nullptr };
		QString hostName_{};
		quint16 readingPort_{ 0x00'ff };
		quint16 sendingPort_{ 0x00'fe };

		std::vector<IValue*> parameters_{};
		std::vector<IValue*>::iterator itCurrent_;

		char lastRWFlag_{ -1 };
		bool lastManualFlag_{ false };
		QString lastId_{};
		QByteArray lastValue_{};
	};

	inline Client::Client(const QString& _uniqueName, 
		const QString _togglePrmID, const QString& _currentStatusPrmID) noexcept :
		Base(_uniqueName, _togglePrmID, _currentStatusPrmID)
	{}

	inline IValue* Client::current() {
		return *itCurrent_;
	}

	inline void Client::next() {
		++itCurrent_;
		if (itCurrent_ == parameters_.end()) { itCurrent_ = parameters_.begin(); }
	}
}
