#pragma once
#include "engine/routine/mock/device/engine_routine_mock_device_base.h"
#include "engine/engine_parameter.h"
#include <QTime>

namespace engine::routine {
	class Base;

	template<RoutineBox T>
	class Factory;

	namespace mock {
		class Server;
	}
}

namespace engine::routine::mock::device {
	class Oscillator :
		public Base
	{
		friend class Server;
		friend class Factory<std::vector<engine::routine::Base*>>;

	private:
		static constexpr double PI = 3.1415926535;

	public:
		virtual ~Oscillator() noexcept override final = default;

	private:
		explicit Oscillator(Parameter<double>*, Parameter<double>*, Parameter<double>*) noexcept;
		Oscillator(const Oscillator&) = delete;

	private:
		virtual void handleParameters() override final;

	private:
		Parameter<double>* pValueRead_{ nullptr };
		Parameter<double>* pAmplitude_{ nullptr };
		Parameter<double>* pPeriod_{ nullptr };

		QTime startTime_;
	};
}
