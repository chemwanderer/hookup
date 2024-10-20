#include "gui_item_mock.h"

namespace gui::item {
	Mock::Mock(const QString& _uniqueName, QWidget* _pParent)
		: Base(_uniqueName, _pParent)
	{
		ui.setupUi(this);
		connect(ui.doubleSpinBoxAmplitude, &QDoubleSpinBox::editingFinished, this, &Mock::slot_setAmplitude);
		connect(ui.doubleSpinBoxPeriod, &QDoubleSpinBox::editingFinished, this, &Mock::slot_setPeriod);
	}

	Mock::~Mock()
	{}

	void Mock::updateReadouts() {
		if (!pAmplitude_ || !pPeriod_ || !pValue_) { return; }

		qreal value = 0.;
		pValue_->submit(&value);
		ui.labelValueRead->setText(QString::number(value, 'f', 3));

		auto* pAmplitudeBox = ui.doubleSpinBoxAmplitude;
		if (!pAmplitudeBox->hasFocus()) {
			qreal amplitude = 0.;
			pAmplitude_->submit(&amplitude);

			pAmplitudeBox->blockSignals(true);
			pAmplitudeBox->setValue(amplitude);
			pAmplitudeBox->blockSignals(false);
		}

		auto* pPeriodBox = ui.doubleSpinBoxPeriod;
		if (!pPeriodBox->hasFocus()) {
			qreal period = 0.;
			pPeriod_->submit(&period);

			pPeriodBox->blockSignals(true);
			pPeriodBox->setValue(period);
			pPeriodBox->blockSignals(false);
		}
	}

	void Mock::slot_setAmplitude() {
		if (!pAmplitude_) { return; }

		qreal amplitude = ui.doubleSpinBoxAmplitude->value();
		pAmplitude_->accept(&amplitude);
	}

	void Mock::slot_setPeriod() {
		if (!pPeriod_) { return; }
		
		qreal period = ui.doubleSpinBoxPeriod->value();
		pPeriod_->accept(&period);
	}
}
