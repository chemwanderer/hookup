#include "kit/kit.h"
#include "engine_routine_inner_oscillation.h"
#include <QElapsedTimer>

namespace engine::routine::inner {
	void Oscillation::run() {
		if (!pControlled_ || !pAmplitude_ || !pPeriod_ || !pOffset_) { return; }

		lastManualControlState_ = pControlled_->manualControl();
		pControlled_->setManualControl(false);

		QElapsedTimer timer;
		timer.start();

		double amplitude = 0., offset = 0., period = 0., value = 0., secs = 0.;
		std::chrono::milliseconds nap(DEFAULT_SLEEP_DURATION);

		while (true) {
			if (!checkToggle()) { return; }

			pAmplitude_->submit(&amplitude);
			pPeriod_->submit(&period);
			pOffset_->submit(&offset);

			if (std::abs(period) > MIN_PERIOD_SECS) {
				secs = static_cast<double>(timer.elapsed()) * 1e-3;
				value = offset + amplitude * sin((2 * kit::pi()) / period * secs);
			}
			else {
				value = 0.;
			}

			pControlled_->accept(&value);
			std::this_thread::sleep_for(nap);
		}
	}

	void Oscillation::beforeQuit() {
		if (pControlled_) { pControlled_->setManualControl(lastManualControlState_); }
	}
}
