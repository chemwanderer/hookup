#include "gui_item_base.h"

namespace gui::item {
	const QString Base::onlineSignature_ = QStringLiteral("online");
	const QString Base::offlineSignature_ = QStringLiteral("offline");

	const QColor Base::onlineColor_{ 34, 177, 76, 25 };
	const QColor Base::offlineColor_{ 255, 127, 39, 25 };
	const QColor Base::neutralColor_{ 127, 127, 127, 25 };
}
