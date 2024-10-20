#include "engine_routine_mock_device_led.h"

namespace engine::routine::mock::device {
	LED::LED(Parameter<ToggleStatus>* _pStateRead, Parameter<bool>* _pIsOn) noexcept :
		pStateRead_(_pStateRead),
		pIsOn_(_pIsOn)
	{}

	void LED::handleParameters() {
		if (!pStateRead_ || !pIsOn_) { return; }

		bool isOn = false;
		pIsOn_->submit(&isOn);

		ToggleStatus stateRead = isOn ? ToggleStatus::on : ToggleStatus::off;
		pStateRead_->accept(&stateRead);
	}
}