#pragma once
#include <QPointF>
#include <QFile>
#include <array>
#include <concepts>

namespace kit {
	static constexpr QStringView __curveFileExtention__ = u".crv";

	template<typename _T>
	concept PointArray = requires(_T _obj, uint32_t _idx, double _value) {
		_obj.end();
		_obj[_idx].setX(_value);
		_obj[_idx].setY(_value);
		static_cast<double>(_obj.begin()->x());
		static_cast<double>(_obj.begin()->y());
		static_cast<double>(_obj[_idx].x());
		static_cast<double>(_obj[_idx].y());
		static_cast<double>(_obj.at(_idx).x());
		static_cast<double>(_obj.at(_idx).y());
		static_cast<std::size_t>(_obj.size());
	};

	consteval double pi();

	template<PointArray _Arr>
	constexpr double simpsonIntegral(const _Arr&);

	template<PointArray _Arr>
	bool saveCurve(const QString& _path, const _Arr& _src);

	template<PointArray _Arr>
	bool loadCurve(const QString& _path, _Arr& _dst);

	template<PointArray _Arr>
	int64_t pointsNumber(const QString& _path);
}

template<kit::PointArray _Arr>
constexpr double kit::simpsonIntegral(const _Arr& _source) {
	std::size_t size = _source.size();
	if (2 > size) { return 0.; }

	constexpr double divider = 1. / 6;

	double result = 0.;

	uint32_t isSizeEven = (~size) & 0x00'00'00'01;
	auto lastIt = _source.end() - 2 - isSizeEven;

	double xl = _source.begin()->x();
	double yl = _source.begin()->y();
	for (auto itLeft = _source.begin(); itLeft < lastIt; itLeft += 2) {
		auto itMedium = itLeft + 1;
		auto itRight = itLeft + 2;
		double xm = itMedium->x();
		double ym = itMedium->y();
		double xr = itRight->x();
		double yr = itRight->y();

		result += divider * (xr - xl) * (yl + 4 * ym + yr);
		xl = xr;
		yl = yr;
	}

	if (static_cast<bool>(isSizeEven)) {
		auto itRight = _source.end() - 1;
		result += 0.5 * (yl + itRight->y()) * (itRight->x() - xl);
	}

	return result;
}

consteval double kit::pi() {
	constexpr std::size_t sectionsNumber = 2000;
	std::array<QPointF, sectionsNumber + 1> grid(QPointF(0., 0.));
	for (std::size_t i = 0; i <= sectionsNumber; ++i) {
		double x = static_cast<double>(i) / sectionsNumber;
		grid[i] = QPointF(x, 1. / (1. + x * x));
	}
	return 4. * simpsonIntegral(grid);
}

template<kit::PointArray _Arr>
bool kit::saveCurve(const QString& _path, const _Arr& _src) {
	QFile output;

	QString editedPath = _path;
	auto dotIt = std::find_if(editedPath.rbegin(), editedPath.rend(), [](auto _symb)->bool { return '.' == _symb; });
	if (editedPath.rend() != dotIt) { editedPath.erase((dotIt.base() - 1), editedPath.end()); }
	editedPath.append(__curveFileExtention__);

	output.setFileName(editedPath);
	if (!output.open(QFile::WriteOnly)) { return false; }

	uint64_t size = _src.size();
	output.write(QByteArray(reinterpret_cast<const char*>(&size), sizeof(uint64_t)));
	for (const auto& point : _src) {
		double x = point.x();
		double y = point.y();
		output.write(QByteArray(reinterpret_cast<const char*>(&x), sizeof(double)));
		output.write(QByteArray(reinterpret_cast<const char*>(&y), sizeof(double)));
	}

	output.close();
	return true;
}

template<kit::PointArray _Arr>
bool kit::loadCurve(const QString& _path, _Arr& _dst) {
	QFile input;
	input.setFileName(_path);
	if (!input.open(QFile::ReadOnly)) { return false; }

	uint64_t curveSize = 0;
	int64_t bytesNumber = input.read(reinterpret_cast<char*>(&curveSize), sizeof(uint64_t));
	if (!bytesNumber || 0 > bytesNumber) { return false; }
	
	double x = 0.;
	double y = 0.;
	for (uint64_t i = 0; i < curveSize && i < _dst.size(); ++i) {
		bytesNumber = input.read(reinterpret_cast<char*>(&x), sizeof(double));
		if (!bytesNumber || 0 > bytesNumber) { return false; }

		bytesNumber = input.read(reinterpret_cast<char*>(&y), sizeof(double));
		if (!bytesNumber || 0 > bytesNumber) { return false; }

		_dst[i].setX(x);
		_dst[i].setY(y);
	}

	input.close();
	return true;
}

template<kit::PointArray _Arr>
int64_t kit::pointsNumber(const QString& _path) {
	QFile input;
	input.setFileName(_path);
	if (!input.open(QFile::ReadOnly)) { return -1; }

	uint64_t size = 0;
	int64_t bytesNumber = input.read(reinterpret_cast<char*>(&size), sizeof(uint64_t));
	if (!bytesNumber || 0 > bytesNumber) { return -1; }

	return static_cast<int64_t>(size);
}
