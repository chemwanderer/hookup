#pragma once

namespace engine::routine {
	template<typename T>
	concept RoutineBox = requires(T _obj) {
		_obj.end();
		_obj.insert(_obj.end(), *(_obj.begin()));
	};

	// routine execution status:
	enum class Status : char
	{
		waiting = 1,
		trigger = 2,
		running = 3,
		braking = 4,
		quitted = 5
	};
}
