#pragma once
#include <concepts>

namespace chart {
	template<typename _T>
	concept Rect = requires(_T _obj, double _value) {
		static_cast<int>(_obj.x());
		static_cast<int>(_obj.y());
		static_cast<int>(_obj.width());
		static_cast<int>(_obj.height());
		_obj.setX(_value);
		_obj.setY(_value);
		_obj.setWidth(_value);
		_obj.setHeight(_value);
	};

	template<typename _Num>
	class Scope
	{
	public:
		Scope() noexcept = default;
		explicit Scope(_Num _minX, _Num _minY, _Num _maxX, _Num _maxY) noexcept;
		~Scope() noexcept = default;

	public:
		_Num minAbscissa() const;
		_Num minOrdinate() const;
		_Num maxAbscissa() const;
		_Num maxOrdinate() const;

		void setMinAbscissa(_Num);
		void setMinOrdinate(_Num);
		void setMaxAbscissa(_Num);
		void setMaxOrdinate(_Num);
		void set(_Num _minX, _Num _minY, _Num _maxX, _Num _maxY);

	private:
		_Num minAbscissaScoped_{ 0. };
		_Num minOrdinateScoped_{ 0. };
		_Num maxAbscissaScoped_{ 1. };
		_Num maxOrdinateScoped_{ 1. };
	};

	template<typename _Num>
	inline Scope<_Num>::Scope(_Num _minX, _Num _minY, _Num _maxX, _Num _maxY) noexcept :
		minAbscissaScoped_(_minX),
		minOrdinateScoped_(_minY),
		maxAbscissaScoped_(_maxX),
		maxOrdinateScoped_(_maxY)
	{}

	template<typename _Num>
	inline _Num Scope<_Num>::minAbscissa() const {
		return minAbscissaScoped_;
	}

	template<typename _Num>
	inline _Num Scope<_Num>::minOrdinate() const {
		return minOrdinateScoped_;
	}

	template<typename _Num>
	inline _Num Scope<_Num>::maxAbscissa() const {
		return maxAbscissaScoped_;
	}

	template<typename _Num>
	inline _Num Scope<_Num>::maxOrdinate() const {
		return maxOrdinateScoped_;
	}

	template<typename _Num>
	inline void Scope<_Num>::setMinAbscissa(_Num _value) {
		minAbscissaScoped_ = _value;
	}

	template<typename _Num>
	inline void Scope<_Num>::setMinOrdinate(_Num _value) {
		minOrdinateScoped_ = _value;
	}

	template<typename _Num>
	inline void Scope<_Num>::setMaxAbscissa(_Num _value) {
		maxAbscissaScoped_ = _value;
	}

	template<typename _Num>
	inline void Scope<_Num>::setMaxOrdinate(_Num _value) {
		maxOrdinateScoped_ = _value;
	}

	template<typename _Num>
	void Scope<_Num>::set(_Num _minX, _Num _minY, _Num _maxX, _Num _maxY) {
		minAbscissaScoped_ = _minX;
		minOrdinateScoped_ = _minY;
		maxAbscissaScoped_ = _maxX;
		maxOrdinateScoped_ = _maxY;
	}
}
