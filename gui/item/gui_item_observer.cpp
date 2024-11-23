#include "gui_item_observer.h"
#include <QTimer>

namespace gui::item {
	Observer::Observer(const QString& _uniqueName, QWidget* _pParent)
		: Base(_uniqueName, _pParent)
	{
		ui.setupUi(this);

		ui.doubleSpinBoxCapturePeriod->blockSignals(true);
		ui.doubleSpinBoxCapturePeriod->setValue(capturePeriod_);
		ui.doubleSpinBoxCapturePeriod->blockSignals(false);

		pCaptureTimer_ = new QTimer(this);
		pCaptureTimer_->setTimerType(Qt::PreciseTimer);
		pCaptureTimer_->setInterval(capturePeriod_);

		model_.addSourceData(QStringLiteral("Observed Data"), &capturedPoints_);
		model_.setScope(0., 0., 1., 1.);
		ui.widgetView->setModel(&model_);

		connect(pCaptureTimer_, &QTimer::timeout, this, &Observer::slot_capture);
		connect(ui.checkBoxCaptureOn, &QCheckBox::toggled, this, &Observer::slot_switch);
		connect(ui.doubleSpinBoxCapturePeriod, &QDoubleSpinBox::editingFinished, this, &Observer::slot_setCapturePeriod);
		connect(ui.pushButtonLoad, &QPushButton::clicked, this, &Observer::slot_loadCurve);
	}

	Observer::~Observer()
	{}

	void Observer::updateReadouts() {
		if (!pValueRead_) { return; }

		if (pFeedbackStatus_) {
			bool isOnline = false;
			pFeedbackStatus_->submit(&isOnline);

			if (isOnline) {
				ui.labelIsOnline->setText(onlineSignature());
				ui.labelIsOnline->setPalette(onlineColor());
			}
			else {
				ui.labelIsOnline->setText(offlineSignature());
				ui.labelIsOnline->setPalette(offlineColor());
			}
		}
		else {
			ui.labelIsOnline->setText(QStringLiteral("-"));
			ui.labelIsOnline->setPalette(neutralColor());
		}

		qreal valueRead = 0.;
		pValueRead_->submit(&valueRead);
		ui.labelCurrent->setText(QString::number(valueRead, 'f', 3));

		if (isRunning_ && capturedPoints_.size() > 1) {
			qreal deltaScope = 0.2 * std::max(std::abs(currentMinValue_), std::abs(currentMaxValue_));
			if (deltaScope < 1e-10) { deltaScope = 1.; }

			model_.setScope(0., currentMinValue_ - deltaScope, capturedPoints_.back().x(), currentMaxValue_ + deltaScope);
			model_.rebuild();

			ui.widgetView->update();
		}
	}

	void Observer::slot_switch(bool _isOn) {
		if (!pValueRead_) { return; }

		if (_isOn && !isRunning_) {
			capturedPoints_.clear();

			startTime_ = QTime::currentTime();

			double startValue = 0.;
			pValueRead_->submit(&startValue);
			capturedPoints_.push_back(std::move(QPointF(0., startValue)));
			currentMinValue_ = startValue;
			currentMaxValue_ = startValue;

			pCaptureTimer_->setInterval(capturePeriod_);
			pCaptureTimer_->start();

			isRunning_ = true;
		}
		else {
			pCaptureTimer_->stop();
			isRunning_ = false;
		}
	}

	void Observer::slot_capture() {
		if (!pValueRead_) { return; }

		qreal nextValue = 0.;
		pValueRead_->submit(&nextValue);
		double nextTime = -static_cast<double>(QTime::currentTime().msecsTo(startTime_)) * 1.0e-3;
		capturedPoints_.push_back(std::move(QPointF(nextTime, nextValue)));

		if (nextValue < currentMinValue_) { currentMinValue_ = nextValue; }
		if (nextValue > currentMaxValue_) { currentMaxValue_ = nextValue; }
	}

	void Observer::slot_setCapturePeriod() {
		capturePeriod_ = ui.doubleSpinBoxCapturePeriod->value();
		pCaptureTimer_->setInterval(capturePeriod_);
	}

	void Observer::slot_loadCurve() {
		if (isRunning_) { return; }

		const QString path = QFileDialog::getOpenFileName(this, QStringLiteral("Open"));
		if (path.isEmpty()) { return; }

		auto pointsNumber = kit::pointsNumber<std::vector<QPointF>>(path);
		if (pointsNumber <= 0) { return; }

		capturedPoints_.clear();
		capturedPoints_.resize(pointsNumber);

		if (kit::loadCurve(path, capturedPoints_)) {
			auto itMax = std::max_element(capturedPoints_.begin(), capturedPoints_.end(),
				[](const QPointF& _p1, const QPointF& _p2)->bool { return _p1.y() < _p2.y(); });
			auto itMin = std::min_element(capturedPoints_.begin(), capturedPoints_.end(),
				[](const QPointF& _p1, const QPointF& _p2)->bool { return _p1.y() < _p2.y(); });

			currentMaxValue_ = itMax->y();
			currentMinValue_ = itMin->y();

			qreal deltaScope = 0.02 * std::max(std::abs(currentMinValue_), std::abs(currentMaxValue_));
			if (deltaScope < 1e-10) { deltaScope = 1.; }

			model_.setScope(capturedPoints_.begin()->x(), currentMinValue_ - deltaScope,
				(capturedPoints_.end() - 1)->x(), currentMaxValue_ + deltaScope);
			model_.rebuild();

			ui.widgetView->update();
		}
	}
}
