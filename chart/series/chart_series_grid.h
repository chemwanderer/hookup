#pragma once
#include "chart/series/chart_series_base.h"

namespace chart::series {
	template<kit::PointArray _SourceVec, Rect _ChartArea>
	class Grid :
		public Base<_SourceVec, _ChartArea>
	{
	public:
		Grid() noexcept = default;
		Grid(const Grid&) = delete;
		virtual ~Grid() noexcept override = default;

	public:
		void setAbscissaSignature(const QString&);
		void setOrdinateSignature(const QString&);

	private:
		virtual void draw(QPainter*) override;
		virtual void rebuild(const Scope<double>*, const _ChartArea*) override;

	private:
		static void calcDivisionValue(double* _pDiv, int* _pPrec, double _deltaArg);

	private:
		int fontSize_{ 9 };
		double divisionValueX_{ 0.5 };
		double divisionValueY_{ 0.5 };
		int precisionX_{ 1 };
		int precisionY_{ 1 };

		double xAxisPos_{ 0. };
		double yAxisPos_{ 0. };
		double xZeroPos_{ 0. };
		double yZeroPos_{ 0. };
		double xAxisLength_{ 0. };
		double yAxisLength_{ 0. };

		std::vector<std::pair<double, double>> xDashes_{};	// source, view
		std::vector<std::pair<double, double>> yDashes_{};	// source, view

		QString xAxisSignature_{ QStringLiteral("[x, units]") };
		QString yAxisSignature_{ QStringLiteral("[y, units]") };

	private:
		static constexpr int PRIMARY_DASH_HALF_LENGTH = 2;
		static constexpr int AXIS_WIDTH = 1;
		static constexpr int MIN_DIST = 50;
	};

	template<kit::PointArray _SourceVec, Rect _ChartArea>
	void Grid<_SourceVec, _ChartArea>::calcDivisionValue(double* _pDiv, int* _pPrec, double _deltaArg) {
		double div = 1.;
		int precision = 1;
		double scale = _deltaArg;
		if (_deltaArg < 1.) {
			while (scale < 1.) {
				scale *= 10;
				div *= 0.1;
				++precision;
			}
		}
		else {
			while (scale > 10. && div < 0.25 * _deltaArg) {
				scale *= 0.1;
				div *= 10.;
			}

			if (div > 0.25 * _deltaArg) {
				div *= 0.5;
			}
		}
		
		*_pDiv = div;
		*_pPrec = precision;
	}

	template<kit::PointArray _SourceVec, Rect _ChartArea>
	void Grid<_SourceVec, _ChartArea>::rebuild(const Scope<double>* _pScope, const _ChartArea* _pChartArea) {
		xDashes_.clear();
		yDashes_.clear();

		xAxisPos_ = static_cast<double>(_pChartArea->y()) + static_cast<double>(_pChartArea->height());
		yAxisPos_ = static_cast<double>(_pChartArea->x());
		xAxisLength_ = static_cast<double>(_pChartArea->width());
		yAxisLength_ = static_cast<double>(_pChartArea->height());

		double minX = _pScope->minAbscissa();
		double maxX = _pScope->maxAbscissa();
		double minY = _pScope->minOrdinate();
		double maxY = _pScope->maxOrdinate();
		double dX = maxX - minX;
		double dY = maxY - minY;

		calcDivisionValue(&divisionValueX_, &precisionX_, dX);
		calcDivisionValue(&divisionValueY_, &precisionY_, dY);

		xZeroPos_ = _pChartArea->x() - minX * _pChartArea->width() / dX;
		yZeroPos_ = _pChartArea->y() + maxY * _pChartArea->height() / dY;

		int nXDivs = static_cast<int>(minX / divisionValueX_) + static_cast<int>(minX > 0);
		for (double x = nXDivs * divisionValueX_; x < maxX; x += divisionValueX_) {
			double xView = Base<_SourceVec, _ChartArea>::xToView(x, _pScope, _pChartArea);
			xDashes_.push_back(std::move(std::make_pair(x, xView)));
		}

		int nYDivs = static_cast<int>(minY / divisionValueY_) + static_cast<int>(minY > 0);
		for (double y = nYDivs * divisionValueY_; y < maxY; y += divisionValueY_) {
			double yView = Base<_SourceVec, _ChartArea>::yToView(y, _pScope, _pChartArea);
			yDashes_.push_back(std::move(std::make_pair(y, yView)));
		}
	}

	template<kit::PointArray _SourceVec, Rect _ChartArea>
	void Grid<_SourceVec, _ChartArea>::draw(QPainter* _pPainter) {
		_pPainter->save();

		QPointF bottomLeft(yAxisPos_, xAxisPos_);
		QPointF topLeft(yAxisPos_, xAxisPos_ - yAxisLength_);
		QPointF bottomRight(yAxisPos_ + xAxisLength_, xAxisPos_);

		double textRectHeight = textRectHeight = fontSize_ * 2.5;
		_pPainter->setFont(QFont(QStringLiteral("Arial"), fontSize_));
		for (auto& dash : xDashes_) {
			_pPainter->setPen(QPen(QBrush(QColor(0, 0, 0, 50)), 1., Qt::DotLine, Qt::RoundCap));
			_pPainter->drawLine(QPointF(dash.second, xAxisPos_), QPointF(dash.second, xAxisPos_ - yAxisLength_));

			QString strNumber = QString::number(dash.first, 'f', precisionX_);
			double textRectWidth = strNumber.size() * fontSize_;

			_pPainter->setPen(Qt::black);
			_pPainter->drawLine(QPointF(dash.second, xAxisPos_ - PRIMARY_DASH_HALF_LENGTH),
				QPointF(dash.second, xAxisPos_ + PRIMARY_DASH_HALF_LENGTH));
			_pPainter->drawText(QRectF(dash.second - 0.5 * textRectWidth, xAxisPos_, textRectWidth, textRectHeight),
				Qt::AlignHCenter | Qt::AlignVCenter, strNumber);
		}
		_pPainter->setPen(Qt::black);
		_pPainter->drawText(QRectF(yAxisPos_, xAxisPos_ + textRectHeight, xAxisLength_, textRectHeight),
			Qt::AlignRight | Qt::AlignVCenter, xAxisSignature_);

		double maxTextRectWidth = 0.;
		for (auto& dash : yDashes_) {
			_pPainter->setPen(QPen(QBrush(QColor(0, 0, 0, 50)), 1., Qt::DotLine, Qt::RoundCap));
			_pPainter->drawLine(QPointF(yAxisPos_, dash.second), QPointF(yAxisPos_ + xAxisLength_, dash.second));

			QString strNumber = QString::number(dash.first, 'f', precisionY_);
			double textRectWidth = strNumber.size() * fontSize_;
			if (textRectWidth > maxTextRectWidth) { maxTextRectWidth = textRectWidth; }

			_pPainter->setPen(Qt::black);
			_pPainter->drawLine(QPointF(yAxisPos_ - PRIMARY_DASH_HALF_LENGTH, dash.second),
				QPointF(yAxisPos_ + PRIMARY_DASH_HALF_LENGTH, dash.second));
			_pPainter->drawText(QRectF(yAxisPos_ - textRectWidth - fontSize_, 
				dash.second - 0.5 * textRectHeight, textRectWidth, textRectHeight),
				Qt::AlignRight | Qt::AlignVCenter, strNumber);
		}
		_pPainter->save();
		_pPainter->translate(yAxisPos_ - 1.75 * maxTextRectWidth, xAxisPos_);
		_pPainter->rotate(-90.);
		_pPainter->setPen(Qt::black);
		_pPainter->drawText(QRect(0, 0, yAxisLength_, textRectHeight),
			Qt::AlignRight | Qt::AlignVCenter, yAxisSignature_);
		_pPainter->restore();

		_pPainter->setPen(QPen(QBrush(QColor(0, 0, 0, 50)), 1., Qt::DotLine, Qt::RoundCap));
		_pPainter->drawRect(QRectF(topLeft, bottomRight));

		_pPainter->setPen(QPen(QBrush(Qt::black), AXIS_WIDTH, Qt::SolidLine, Qt::RoundCap));
		_pPainter->drawLine(bottomLeft, bottomRight);
		_pPainter->drawLine(bottomLeft, topLeft);

		_pPainter->restore();
	}

	template<kit::PointArray _SourceVec, Rect _ChartArea>
	inline void Grid<_SourceVec, _ChartArea>::setAbscissaSignature(const QString& _xAxisSignature) {
		xAxisSignature_ = _xAxisSignature;
	}

	template<kit::PointArray _SourceVec, Rect _ChartArea>
	inline void Grid<_SourceVec, _ChartArea>::setOrdinateSignature(const QString& _yAxisSignature) {
		yAxisSignature_ = _yAxisSignature;
	}
}
