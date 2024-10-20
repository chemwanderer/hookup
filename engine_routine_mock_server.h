#pragma once
#include "engine/routine/engine_routine_base.h"
#include "engine/routine/mock/device/engine_routine_mock_device_base.h"

namespace engine::routine {
	class Base;

	template<RoutineBox T>
	class Factory;
}

namespace engine::routine::mock {
	class Server :
		public Base
	{
		friend class Factory<std::vector<Base*>>;

	public:
		virtual ~Server() noexcept override final;
		Server(const Server&) = delete;

	private:
		explicit Server(const QString& _uniqueName, const QString _togglePrmID, const QString& _currentStatusPrmID) noexcept;

	private:
		virtual void run() override final;
		virtual void beforeQuit() override final;

	private:
		device::Base* currentDevice();
		void nextDevice();

	private:
		std::vector<device::Base*> devices_{};
		std::vector<device::Base*>::iterator itCurrentDevice_;
	};

	inline device::Base* Server::currentDevice() {
		return *itCurrentDevice_;
	}

	inline void Server::nextDevice() {
		++itCurrentDevice_;
		if (devices_.end() == itCurrentDevice_) { itCurrentDevice_ = devices_.begin(); }
	}
}