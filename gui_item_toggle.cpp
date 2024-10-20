#include "gui_item_toggle.h"

namespace gui::item {
	Toggle::Toggle(const QString& _uniqueName, QWidget* _pParent)
		: Base(_uniqueName, _pParent)
	{
		ui.setupUi(this);
		connect(ui.checkBoxStateEst, &QCheckBox::toggled, this, &Toggle::slot_setState);
	}

	Toggle::~Toggle()
	{}

	void Toggle::updateReadouts() {
		if (!pStateEst_) { return; }

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

		if (pStateRead_) {
			engine::ToggleStatus stateRead = engine::ToggleStatus::undefined;
			pStateRead_->submit(&stateRead);

			switch (stateRead) {
			case engine::ToggleStatus::on: {
				ui.labelStateRead->setText(QStringLiteral("on"));
				break;
			}
			case engine::ToggleStatus::off: {
				ui.labelStateRead->setText(QStringLiteral("off"));
				break;
			}
			default: {
				ui.labelStateRead->setText(QStringLiteral("undefined"));
			}
			}
		}
		else {
			ui.labelStateRead->setText(QStringLiteral("-"));
		}

		auto* pCheckBox = ui.checkBoxStateEst;
		if (!pCheckBox->hasFocus()) {
			bool isOn = false;
			pStateEst_->submit(&isOn);

			pCheckBox->blockSignals(true);
			pCheckBox->setChecked(isOn);
			pCheckBox->blockSignals(false);
		}
	}

	void Toggle::slot_setState(bool _state) {
		if (!pStateEst_) { return; }
		pStateEst_->accept(&_state);
	}
}
