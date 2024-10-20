#pragma once
#include <vector>
#include "engine/routine/engine_routine_common.h"

namespace engine::routine {
	class Base;

	template<RoutineBox T>
	class Factory;

	namespace mock {
		class Server;
	}
}

namespace engine::routine::mock::device {
	class Base 
	{
		friend class Server;
		friend class Factory<std::vector<engine::routine::Base*>>;

	public:
		virtual ~Base() noexcept = default;

	protected:
		virtual void handleParameters() = 0;
	};
}