#include "chart_view.h"
#include <QResizeEvent>

namespace chart {
	View::View(QWidget* _pParent) :
		QWidget(_pParent)
	{
		chartRect_.setCoords(LEFT_FIELD, TOP_FIELD,
			width() - RIGHT_FIELD, height() - BOTTOM_FIELD);

		setMinimumSize(LEFT_FIELD + RIGHT_FIELD, TOP_FIELD + BOTTOM_FIELD);
	}

	void View::paintEvent(QPaintEvent* _pEvent) {
		if (!pModel_) return;

		QPainter painter;
		painter.begin(this);
		painter.setRenderHint(QPainter::Antialiasing);

		painter.fillRect(chartRect_, QColor(Qt::white));
		pModel_->draw(&painter);

		painter.end();
	}

	void View::resizeEvent(QResizeEvent* _pEvent) {
		chartRect_.setCoords(LEFT_FIELD, TOP_FIELD,
			width() - RIGHT_FIELD, height() - BOTTOM_FIELD);

		if (!pModel_) { return; }
		pModel_->setChartArea(chartRect_.x(), chartRect_.y(), chartRect_.width(), chartRect_.height());
	}

	void View::setModel(IModel* _pModel) {
		pModel_ = _pModel;
		if (!pModel_) { return; }

		pModel_->setChartArea(chartRect_.x(), chartRect_.y(), chartRect_.width(), chartRect_.height());
	}
}
