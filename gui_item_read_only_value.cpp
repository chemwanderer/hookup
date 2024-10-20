#include "gui_item_read_only_value.h"

namespace gui::item {
	ReadOnlyValue::ReadOnlyValue(const QString& _uniqueName, QWidget* _pParent)
		: Base(_uniqueName, _pParent)
	{
		ui.setupUi(this);
		ui.labelIsOnline->setAutoFillBackground(true);
	}

	ReadOnlyValue::~ReadOnlyValue()
	{}

	void ReadOnlyValue::updateReadouts() {
		if (!pValue_) { return; }

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

		qreal value = 0.;
		pValue_->submit(&value);

		switch (format_) {
		case Format::floating: {
			ui.labelValueRead->setText(QString::number(value, 'f', precision_));
			break;
		}
		case Format::exponential: {
			ui.labelValueRead->setText(QString::number(value, 'e', precision_));
			break;
		}
		}
	}
}
