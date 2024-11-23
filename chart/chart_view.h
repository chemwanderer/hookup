#pragma once
#include <QWidget>
#include "chart/chart_i_model.h"

namespace chart {
	class View :
		public QWidget
	{
		Q_OBJECT

	public:
		explicit View(QWidget* _pParent = nullptr);
		virtual ~View() override = default;

	public:
		void setModel(IModel*);

	private:
		virtual void paintEvent(QPaintEvent*) override;
		virtual void resizeEvent(QResizeEvent*) override;

	private:
		IModel* pModel_{ nullptr };
		QRectF chartRect_;

	private:
		static constexpr qreal
			LEFT_FIELD = 70.,
			RIGHT_FIELD = 30.,
			TOP_FIELD = 50.,
			BOTTOM_FIELD = 50.;
	};
}
