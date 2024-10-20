#include "engine_routine_mock_device_oscillator.h"

namespace engine::routine::mock::device {
	Oscillator::Oscillator(Parameter<double>* _pValueRead, 
		Parameter<double>* _pAmplitude, Parameter<double>* _pPeriod) noexcept :
		pValueRead_(_pValueRead),
		pAmplitude_(_pAmplitude),
		pPeriod_(_pPeriod),
		startTime_(QTime::currentTime())
	{}

	void Oscillator::handleParameters() {
		if (!pValueRead_ || !pAmplitude_ || !pPeriod_) { return; }

		double secs = static_cast<double>(QTime::currentTime().msecsTo(startTime_)) * 1.0e-3;

		double amplitude = 0.;
		pAmplitude_->submit(&amplitude);

		double period = 0.;
		pPeriod_->submit(&period);

		double value = 0.;
		if (std::abs(period) > 1e-3) {
			value = amplitude * sin((2 * PI) / period * secs);
		}
		pValueRead_->accept(&value);
	}
}