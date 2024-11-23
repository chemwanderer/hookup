#pragma once
#include <QPainter>
#include "kit/kit.h"
#include "chart/chart_common.h"

namespace chart {
	template<kit::PointArray _SourceVec, Rect _ChartArea>
	class Model;
}

namespace chart::series {
	template<kit::PointArray _SourceVec, Rect _ChartArea>
	class Base
	{
		friend class Model<_SourceVec, _ChartArea>;

	public:
		virtual ~Base() noexcept = default;

	protected:
		virtual void draw(QPainter*) = 0;
		virtual void rebuild(const Scope<double>*, const _ChartArea*) = 0;

	protected:
		static double xToView(double _x, const Scope<double>* _pScope, const _ChartArea* _pChartArea);
		static double yToView(double _y, const Scope<double>* _pScope, const _ChartArea* _pChartArea);
	};

	template<kit::PointArray _SourceVec, Rect _ChartArea>
	inline double Base<_SourceVec, _ChartArea>::xToView(double _x, 
		const Scope<double>* _pScope, const _ChartArea* _pChartArea) {
		return _pChartArea->x() + 
			_pChartArea->width() * (_x - _pScope->minAbscissa()) / (_pScope->maxAbscissa() - _pScope->minAbscissa());
	}

	template<kit::PointArray _SourceVec, Rect _ChartArea>
	inline double Base<_SourceVec, _ChartArea>::yToView(double _y,
		const Scope<double>* _pScope, const _ChartArea* _pChartArea) {
		return _pChartArea->y() +
			_pChartArea->height() * (_pScope->maxOrdinate() - _y) / (_pScope->maxOrdinate() - _pScope->minOrdinate());
	}
}
