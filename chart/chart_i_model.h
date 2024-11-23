#pragma once
#include <QPainter>

namespace chart {
	class IModel
	{
	public:
		virtual ~IModel() noexcept = default;

	public:
		virtual void setChartArea(double _x, double _y, double _width, double _height) = 0;
		virtual void draw(QPainter*) = 0;
		virtual void rebuild() = 0;
	};
}
