#pragma once
#include <cstdint>
#include <concepts>

namespace engine {
	enum class ToggleStatus : char 
	{
		off = 0,
		on = 1,
		undefined = -1
	};

	template<typename T>
	concept Container = requires(T _obj) {
		_obj.end();
		_obj.insert(_obj.end(), *(_obj.begin()));
	};
}