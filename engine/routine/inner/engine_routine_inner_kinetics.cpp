#include "engine_routine_inner_kinetics.h"

namespace engine::routine::inner {
	void Kinetics::run() {
		if (!pCurrentADC_ || !pFullTransmittanceADC_ || !pTransmittance_) { return; }

		QElapsedTimer timer;

		double zeroADC = 0.;
		pFullTransmittanceADC_->submit(&zeroADC);
		if (zeroADC < 1. / 0xff'ff) { return; }

		double currentADC = 0.;
		double transmittance = 0.;
		double time = 0.;
		std::chrono::milliseconds nap(period_);

		double acqTimeMins = -1.;
		if (pRecordTime_) { pRecordTime_->submit(&acqTimeMins); }

		constexpr double secToMin = 1. / 60.;

		curvePoints_.clear();

		timer.start();
		while (true) {
			if (!checkToggle()) { return; }

			pCurrentADC_->submit(&currentADC);
			time = static_cast<double>(timer.elapsed()) * 1e-3;
			//transmittance = 100. * currentADC / zeroADC;
			transmittance = 100 * exp(zeroADC - currentADC);
			pTransmittance_->accept(&transmittance);

			curvePoints_.push_back(QPointF(time, transmittance));

			if (time * secToMin > acqTimeMins && acqTimeMins > 0.) { return; }
			std::this_thread::sleep_for(nap);
		}
	}

	void Kinetics::beforeQuit() {
		kit::saveCurve(__outputDir__.toString() + QStringLiteral("record_") +
			QTime::currentTime().toString(QStringLiteral("hh_mm")), curvePoints_);
	}
}
