#pragma once
#include <vector>
#include "kit/kit.h"
#include "chart/series/chart_series_base.h"
#include "chart/series/chart_series_grid.h"
#include "chart/series/chart_series_polyline.h"
#include "chart/chart_i_model.h"

namespace chart {
	template<kit::PointArray _SourceVec, Rect _ChartArea>
	class Model :
		public IModel
	{
	public:
		void addSourceData(const QString&, const _SourceVec*);
		void addSourceData(QString&&, const _SourceVec*);
		void setScope(double _minX, double _minY, double _maxX, double _maxY);
		void setSignatures(const QString& _abscissaSignature, const QString& _ordinateSignature);

	protected:
		virtual void config();

	public:
		virtual void setChartArea(double _x, double _y, double _width, double _height);
		virtual void draw(QPainter*) override;
		virtual void rebuild() override;

	public:
		Model() noexcept;
		Model(const Model&) = delete;
		virtual ~Model() noexcept;

	private:
		_ChartArea chartArea_;
		Scope<double> currentScope_;

		std::vector<std::tuple<QString, const _SourceVec*>> bank_{};
		std::vector<series::Base<_SourceVec, _ChartArea>*> series_{};
	};

	template<kit::PointArray _T, Rect _U>
	Model<_T, _U>::Model() noexcept { config(); }

	template<kit::PointArray _T, Rect _U>
	Model<_T, _U>::~Model() noexcept
	{
		for (auto* pSeries : series_) {
			delete pSeries;
		}
	}

	template<kit::PointArray _T, Rect _U>
	void Model<_T, _U>::config() {
		auto* pGrid = new series::Grid<_T, _U>;
		series_.push_back(pGrid);
	}

	template<kit::PointArray T, Rect U>
	void Model<T, U>::rebuild() {
		for (auto* pSeries : series_) {
			pSeries->rebuild(&currentScope_, &chartArea_);
		}
	}

	template<kit::PointArray T, Rect U>
	void Model<T, U>::draw(QPainter* _pPainter) {
		for (auto* pSeries : series_) {
			pSeries->draw(_pPainter);
		}
	}

	template<kit::PointArray _, Rect _ChartArea>
	void Model<_, _ChartArea>::setChartArea(double _x, double _y, double _width, double _height) {
		chartArea_.setX(_x);
		chartArea_.setY(_y);
		chartArea_.setWidth(_width);
		chartArea_.setHeight(_height);
		rebuild();
	}

	template<kit::PointArray _SourceVec, Rect _ChartArea>
	void Model<_SourceVec, _ChartArea>::addSourceData(const QString& _name, const _SourceVec* _pSourceData) {
		bank_.push_back(std::make_tuple(_name, _pSourceData));

		auto* pPolyline = new series::Polyline<_SourceVec, _ChartArea>;
		pPolyline->setSourceData(_pSourceData);
		series_.push_back(pPolyline);

		rebuild();
	}

	template<kit::PointArray _SourceVec, Rect _ChartArea>
	void Model<_SourceVec, _ChartArea>::addSourceData(QString&& _name, const _SourceVec* _pSourceData) {
		bank_.push_back(std::make_tuple(_name, _pSourceData));

		auto* pPolyline = new series::Polyline<_SourceVec, _ChartArea>;
		pPolyline->setSourceData(_pSourceData);
		series_.push_back(pPolyline);

		rebuild();
	}

	template<kit::PointArray _T, Rect _U>
	void Model<_T, _U>::setScope(double _minX, double _minY, double _maxX, double _maxY) {
		currentScope_.set(_minX, _minY, _maxX, _maxY);
	}

	template<kit::PointArray _T, Rect _U>
	void Model<_T, _U>::setSignatures(const QString& _abscissaSignature, const QString& _ordinateSignature) {
		auto itSeries = std::find_if(series_.begin(), series_.end(),
			[](const series::Base<_T, _U>* _pSeries)->bool { return dynamic_cast<const series::Grid<_T, _U>*>(_pSeries); });

		if (series_.end() == itSeries) { return; }

		auto* pGrid = dynamic_cast<series::Grid<_T, _U>*>(*itSeries);
		pGrid->setAbscissaSignature(_abscissaSignature);
		pGrid->setOrdinateSignature(_ordinateSignature);
	}
}
