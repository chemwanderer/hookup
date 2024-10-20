#pragma once
#include "engine/routine/mock/device/engine_routine_mock_device_base.h"
#include "engine/engine_parameter.h"

namespace engine::routine {
	class Base;

	template<RoutineBox T>
	class Factory;

	namespace mock {
		class Server;
	}
}

namespace engine::routine::mock::device {
	class LED :
		public Base
	{
		friend class Server;
		friend class Factory<std::vector<engine::routine::Base*>>;

	public:
		virtual ~LED() noexcept override final = default;

	private:
		explicit LED(Parameter<ToggleStatus>*, Parameter<bool>*) noexcept;
		LED(const LED&) = delete;

	private:
		virtual void handleParameters() override final;

	private:
		Parameter<ToggleStatus>* pStateRead_{ nullptr };
		Parameter<bool>* pIsOn_{ nullptr };
	};
}