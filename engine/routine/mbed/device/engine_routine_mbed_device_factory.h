#pragma once
#include "engine/routine/engine_routine_common.h"
#include <vector>
#include <QJsonArray>
#include <QJsonObject>

namespace engine::routine {
	class Base;

	template<RoutineBox T>
	class Factory;
}

namespace engine::routine::mbed::device {
	class Base;

	class Factory
	{
		using DeviceBox = std::vector<Base*>;
		friend class engine::routine::Factory<std::vector<engine::routine::Base*>>;

	private:
		Factory() noexcept = default;
		explicit Factory(DeviceBox*, const QJsonArray*) noexcept;
		Factory(const Factory&) = delete;
		~Factory();

	private:
		DeviceBox* pDeviceBox_{ nullptr };
		const QJsonArray* pDevicesInfo_{};

	private:
		static Base* create(const QJsonObject&);

	private:
		static Base* createToggle(const QJsonObject&);
		static Base* createADC(const QJsonObject&);
		static Base* createMCP4725(const QJsonObject&);
	};
}
