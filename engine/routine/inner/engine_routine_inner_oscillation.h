#pragma once
//////////////////////////////////////////////////////////////
//							    //
// engine::routine::inner::Oscillation is a test routine    //
// changing the selected parameter in time in accordance    //
// with the following sinusoidal rule:			    //
//							    //
// v(t) = A * sin((2 * pi / T) * t) + b,		    //
//						 	    //
// where						    //
// v is the selected parameter value;			    //
// t is the time from the beginning;			    //
// A is the amplitude;					    //
// T is the oscillation period;				    //
// b is the offset;					    //
// pi is the number π.					    //
//							    //
// Can be used for changing voltage level sended to DAC in  //
// time, for example.					    //
//							    //
//////////////////////////////////////////////////////////////
#include <QTime>
#include "engine/routine/engine_routine_base.h"
#include "engine/engine_parameter.h"

namespace engine::routine {
	class Base;

	template<RoutineBox _T>
	class Factory;
}

namespace engine::routine::inner {
	class Oscillation :
		public Base
	{
		friend class Factory<std::vector<Base*>>;

	public:
		virtual ~Oscillation() noexcept override final = default;
		Oscillation(const Oscillation&) = delete;

	private:
		explicit Oscillation(const QString& _uniqueName, 
			const QString _togglePrmID, const QString& _currentStatusPrmID) noexcept;

	private:
		virtual void run() override final;
		virtual void beforeQuit() override final;

	private:
		Parameter<double>* pControlled_{ nullptr };
		const Parameter<double>* pAmplitude_{ nullptr };
		const Parameter<double>* pOffset_{ nullptr };
		const Parameter<double>* pPeriod_{ nullptr };

		bool lastManualControlState_{ false };

	private:
		static constexpr uint32_t DEFAULT_SLEEP_DURATION = 50; //ms
		static constexpr double MIN_PERIOD_SECS = 0.1;
	};

	inline Oscillation::Oscillation(const QString& _uniqueName,
		const QString _togglePrmID, const QString& _currentStatusPrmID) noexcept :
		Base(_uniqueName, _togglePrmID, _currentStatusPrmID)
	{}
}
