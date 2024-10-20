#include "gui_item_controlled_value.h"

namespace gui::item {
	ControlledValue::ControlledValue(const QString& _uniqueName, QWidget* _pParent)
		: Base(_uniqueName, _pParent)
	{
		ui.setupUi(this);
		connect(ui.lineEditValueEst, &QLineEdit::editingFinished, this, &ControlledValue::slot_setValue);
	}

	ControlledValue::~ControlledValue()
	{}

	void ControlledValue::updateReadouts() {
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

		if (pValueRead_) {
			qreal valueRead = 0.;
			pValueRead_->submit(&valueRead);
			ui.labelValueRead->setText(QString::number(valueRead, 'f', precision_));
		}
		else {
			ui.labelValueRead->setText(QStringLiteral(""));
		}

		if (pValueEst_) {
			ui.lineEditValueEst->setEnabled(pValueEst_->manualControl());
		}

		if (pValueEst_ && !ui.lineEditValueEst->hasFocus()) {
			qreal valueEst = 0.;
			pValueEst_->submit(&valueEst);

			ui.lineEditValueEst->blockSignals(true);
			ui.lineEditValueEst->setText(QString::number(valueEst, 'f', precision_));
			ui.lineEditValueEst->blockSignals(false);
		}
	}

	void ControlledValue::slot_setValue() {
		if (!pValueEst_) { return; }

		bool ok = false;
		qreal newValueEst = ui.lineEditValueEst->text().toDouble(&ok);

		if (ok) { pValueEst_->accept(&newValueEst); }
		else {
			qreal currentValueEst = 0.;
			pValueEst_->submit(&currentValueEst);

			ui.lineEditValueEst->blockSignals(true);
			ui.lineEditValueEst->setText(QString::number(currentValueEst, 'f', precision_));
			ui.lineEditValueEst->blockSignals(false);
		}
	}
}
