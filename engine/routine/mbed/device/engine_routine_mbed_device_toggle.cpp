#include "engine_routine_mbed_device_toggle.h"

namespace engine::routine::mbed::device {
	QByteArray Toggle::request() const {
		if (!pIsOn_) { return QByteArray(); }

		bool isOn = false;
		pIsOn_->submit(&isOn);

		if (prevState_ == isOn) {
			return QByteArray(1, READ_STATE_COMMAND);
		}

		QByteArray res(2, SET_STATE_COMMAND);
		*(res.data() + 1) = isOn ? 0x01 : 0x00;
		prevState_ = isOn;

		return res;
	}

	bool Toggle::receive(const QByteArray& _package) {
		if (!pStateRead_ || RECEIVED_PACKAGE_SIZE != _package.size()) { return false; }

		bool ok = false;
		ToggleStatus stateRead = ToggleStatus::undefined;

		const char* pData = _package.data();
		if (READ_STATE_COMMAND == *pData || SET_STATE_COMMAND == *pData) {
			++pData;
			stateRead = (*pData) ? ToggleStatus::on : ToggleStatus::off;
			ok = true;
		}

		pStateRead_->accept(&stateRead);
		return ok;
	}
}
