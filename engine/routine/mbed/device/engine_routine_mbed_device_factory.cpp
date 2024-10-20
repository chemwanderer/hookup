#include "engine_routine_mbed_device_factory.h"

#include "engine/engine_terminal.h"
#include "engine/engine_parameter.h"
#include "engine/pact/engine_pact.h"

#include "engine/routine/mbed/device/engine_routine_mbed_device_toggle.h"
#include "engine/routine/mbed/device/engine_routine_mbed_device_adc.h"
#include "engine/routine/mbed/device/engine_routine_mbed_device_mcp4725.h"

namespace engine::routine::mbed::device {
	Factory::Factory(DeviceBox* _pDeviceBox, const QJsonArray* _pDevicesInfo) noexcept :
		pDeviceBox_(_pDeviceBox),
		pDevicesInfo_(_pDevicesInfo)
	{}

	Factory::~Factory()
	{
		if (!pDeviceBox_ || !pDevicesInfo_) { return; }

		for (auto valueRef : *pDevicesInfo_) {
			if (valueRef.isObject()) {
				auto* pDevice = create(valueRef.toObject());
				if (pDevice) { pDeviceBox_->insert(pDeviceBox_->end(), pDevice); }
			}
		}
	}

	Base* Factory::create(const QJsonObject& _info) {
		if (!_info.contains(pact::routine::mbed::device::__typeKey__)) { return nullptr; }
		if (!_info.contains(pact::routine::mbed::device::__idKey__)) { return nullptr; }

		QString type = _info.value(pact::routine::mbed::device::__typeKey__).toString();
		if (pact::routine::mbed::device::toggle::__type__ == type) { return createToggle(_info); }
		if (pact::routine::mbed::device::adc::__type__ == type) { return createADC(_info); }
		if (pact::routine::mbed::device::mcp4725::__type__ == type) { return createMCP4725(_info); }

		return nullptr;
	}

	Base* Factory::createMCP4725(const QJsonObject& _info) {
		if (!_info.contains(pact::routine::mbed::device::__idKey__)) { return nullptr; }
		if (!_info.value(pact::routine::mbed::device::__idKey__).isDouble()) { return nullptr; }

		quint8 id = static_cast<quint8>(_info.value(pact::routine::mbed::device::__idKey__).toInt());
		QString feedbackStatusID = _info.value(pact::routine::mbed::device::__feedbackStatusID__).toString();
		auto* pDAC = new MCP4725(id, feedbackStatusID);

		if (_info.contains(pact::routine::mbed::device::mcp4725::__valueEstPrmID__)) {
			QString valueEstID = _info.value(pact::routine::mbed::device::mcp4725::__valueEstPrmID__).toString();
			auto* pValueEst = dynamic_cast<Parameter<qreal>*>(Terminal::parameter(valueEstID));
			pDAC->pValueEst_ = pValueEst;
		}

		return pDAC;
	}

	Base* Factory::createADC(const QJsonObject& _info) {
		if (!_info.contains(pact::routine::mbed::device::__idKey__)) { return nullptr; }
		if (!_info.value(pact::routine::mbed::device::__idKey__).isDouble()) { return nullptr; }

		uchar id = static_cast<uchar>(_info.value(pact::routine::mbed::device::__idKey__).toInt());
		QString feedbackStatusID = _info.value(pact::routine::mbed::device::__feedbackStatusID__).toString();

		auto* pADC = new ADC(id, feedbackStatusID);
		if (_info.contains(pact::routine::mbed::device::adc::__channel0PrmID__)) {
			QString prmID = _info.value(pact::routine::mbed::device::adc::__channel0PrmID__).toString();
			auto* pPrm = dynamic_cast<Parameter<qreal>*>(Terminal::parameter(prmID));
			pADC->channels_[0] = pPrm;
		}
		if (_info.contains(pact::routine::mbed::device::adc::__channel1PrmID__)) {
			QString prmID = _info.value(pact::routine::mbed::device::adc::__channel1PrmID__).toString();
			auto* pPrm = dynamic_cast<Parameter<qreal>*>(Terminal::parameter(prmID));
			pADC->channels_[1] = pPrm;
		}
		if (_info.contains(pact::routine::mbed::device::adc::__channel2PrmID__)) {
			QString prmID = _info.value(pact::routine::mbed::device::adc::__channel2PrmID__).toString();
			auto* pPrm = dynamic_cast<Parameter<qreal>*>(Terminal::parameter(prmID));
			pADC->channels_[2] = pPrm;
		}
		if (_info.contains(pact::routine::mbed::device::adc::__channel3PrmID__)) {
			QString prmID = _info.value(pact::routine::mbed::device::adc::__channel3PrmID__).toString();
			auto* pPrm = dynamic_cast<Parameter<qreal>*>(Terminal::parameter(prmID));
			pADC->channels_[3] = pPrm;
		}
		if (_info.contains(pact::routine::mbed::device::adc::__channel4PrmID__)) {
			QString prmID = _info.value(pact::routine::mbed::device::adc::__channel4PrmID__).toString();
			auto* pPrm = dynamic_cast<Parameter<qreal>*>(Terminal::parameter(prmID));
			pADC->channels_[4] = pPrm;
		}
		if (_info.contains(pact::routine::mbed::device::adc::__channel5PrmID__)) {
			QString prmID = _info.value(pact::routine::mbed::device::adc::__channel5PrmID__).toString();
			auto* pPrm = dynamic_cast<Parameter<qreal>*>(Terminal::parameter(prmID));
			pADC->channels_[5] = pPrm;
		}

		return pADC;
	}

	Base* Factory::createToggle(const QJsonObject& _info) {
		if (!_info.contains(pact::routine::mbed::device::__idKey__)) { return nullptr; }
		if (!_info.value(pact::routine::mbed::device::__idKey__).isDouble()) { return nullptr; }

		uchar id = static_cast<uchar>(_info.value(pact::routine::mbed::device::__idKey__).toInt());
		QString stateReadID = _info.value(pact::routine::mbed::device::toggle::__stateReadID__).toString();
		QString stateEstID = _info.value(pact::routine::mbed::device::toggle::__stateEstID__).toString();
		QString feedbackStatusID = _info.value(pact::routine::mbed::device::__feedbackStatusID__).toString();

		auto* pStateReadPrm = dynamic_cast<Parameter<ToggleStatus>*>(Terminal::parameter(stateReadID));
		auto* pStateEstPrm = dynamic_cast<Parameter<bool>*>(Terminal::parameter(stateEstID));

		auto* pToggleDevice = new Toggle(id, feedbackStatusID);
		pToggleDevice->pStateRead_ = pStateReadPrm;
		pToggleDevice->pIsOn_ = pStateEstPrm;

		if (pStateEstPrm) {
			pStateEstPrm->submit(&pToggleDevice->prevState_);
		}

		return pToggleDevice;
	}
}
