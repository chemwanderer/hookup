#pragma once
#include "chart/series/chart_series_base.h"

namespace chart::series {
	template<kit::PointArray _SourceVec, Rect _ChartArea>
	class Polyline :
		public Base<_SourceVec, _ChartArea>
	{
		using DataBank = std::vector<std::tuple<QString, const _SourceVec*>>;

	public:
		void setSourceData(const _SourceVec*);

	public:
		Polyline() noexcept = default;
		Polyline(const Polyline&) = delete;
		virtual ~Polyline() noexcept override = default;

	private:
		virtual void draw(QPainter*) override;
		virtual void rebuild(const Scope<double>*, const _ChartArea*) override;

	private:
		const _SourceVec* pSourceData_{ nullptr };
		QColor color_;
		double lineWidth_{ 1. };

		std::vector<QPointF> pointViews_{};
		QRectF clipRect_;
	};

	template<kit::PointArray _SourceVec, Rect _ChartArea>
	void Polyline<_SourceVec, _ChartArea>::setSourceData(const _SourceVec* _pSourceData) {
		pSourceData_ = _pSourceData;
	}

	template<kit::PointArray _SourceVec, Rect _ChartArea>
	void Polyline<_SourceVec, _ChartArea>::rebuild(const Scope<double>* _pScope, const _ChartArea* _pChartArea) {
		pointViews_.resize(pSourceData_->size());

		double xMin = _pScope->minAbscissa();
		double dx = _pScope->maxAbscissa() - _pScope->minAbscissa();
		double yMax = _pScope->maxOrdinate();
		double dy = _pScope->maxOrdinate() - _pScope->minOrdinate();

		for (uint32_t i = 0; i < pSourceData_->size(); ++i) {
			double xView = Base<_SourceVec, _ChartArea>::xToView(pSourceData_->at(i).x(), _pScope, _pChartArea);
			double yView = Base<_SourceVec, _ChartArea>::yToView(pSourceData_->at(i).y(), _pScope, _pChartArea);
			pointViews_[i] = std::move(QPointF(xView, yView));
		}

		clipRect_.setX(_pChartArea->x());
		clipRect_.setY(_pChartArea->y());
		clipRect_.setWidth(_pChartArea->width());
		clipRect_.setHeight(_pChartArea->height());
	}

	template<kit::PointArray _SourceVec, Rect _ChartArea>
	void Polyline<_SourceVec, _ChartArea>::draw(QPainter* _pPainter) {
		_pPainter->save();

		_pPainter->setPen(QPen(QBrush(color_), lineWidth_, Qt::SolidLine, Qt::RoundCap));
		_pPainter->setClipRect(clipRect_);
		_pPainter->drawPolyline(pointViews_.data(), pointViews_.size());

		_pPainter->restore();
	}
}
