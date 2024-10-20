#pragma once
#include "engine/routine/engine_routine_base.h"
#include <QUdpSocket>

namespace engine::routine {
	class Base;

	template<RoutineBox T>
	class Factory;
}

namespace engine::routine::udp {
	class Server :
		public Base
	{
		friend class Factory<std::vector<Base*>>;

	private:
		static constexpr qint32 ATTEMPT_DURATION = 10;

	public:
		virtual ~Server() noexcept override final = default;

	private:
		explicit Server(const QString& _uniqueName,
			const QString _togglePrmID, const QString& _currentStatusPrmID) noexcept;

	private:
		virtual void run() override final;
		virtual void beforeQuit() override final;

	private:
		bool waitForRequest();

	private:
		QUdpSocket* pReadingSocket_{ nullptr };
		QUdpSocket* pSendingSocket_{ nullptr };
		QString hostName_{};
		quint16 readingPort_{ 0x00'fe };
		quint16 sendingPort_{ 0x00'ff };

		char lastRWFlag_{ -1 };
		bool lastManualFlag_{ false };
		QString lastId_{};
		QByteArray lastValue_{};
	};

	inline Server::Server(const QString& _uniqueName,
		const QString _togglePrmID, const QString& _currentStatusPrmID) noexcept :
		Base(_uniqueName, _togglePrmID, _currentStatusPrmID)
	{}
}
