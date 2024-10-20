#pragma once
#include "engine/engine_terminal.h"
#include "engine/routine/engine_routine_common.h"
#include "engine/pact/engine_pact.h"

#include "engine/routine/mock/engine_routine_mock_server.h"
#include "engine/routine/mock/device/engine_routine_mock_device_led.h"
#include "engine/routine/mock/device/engine_routine_mock_device_oscillator.h"

#include "engine/routine/mbed/engine_routine_mbed_server.h"
#include "engine/routine/mbed/device/engine_routine_mbed_device_factory.h"

#include "engine/routine/inner/engine_routine_inner_oscillation.h"

#include "engine/routine/udp/engine_routine_udp_client.h"
#include "engine/routine/udp/engine_routine_udp_server.h"

#include <QJsonObject>
#include <QJsonArray>

namespace engine::routine {
	class Base;

	template<RoutineBox T>
	class Factory
	{
	public:
		explicit Factory(T*, const QJsonArray*) noexcept;
		Factory(const Factory&) = delete;
		~Factory();

	private:
		Factory() noexcept = default;

	private:
		T* pRoutineBox_{ nullptr };
		const QJsonArray* pRoutinesInfo_{};

	private:
		static Base* create(const QJsonObject&);

	private:
		static Base* createMock(const QJsonObject&);
		static Base* createMbed(const QJsonObject&);
		static Base* createOscillation(const QJsonObject&);
		static Base* createUdpClient(const QJsonObject&);
		static Base* createUdpServer(const QJsonObject&);
	};

	template<RoutineBox T>
	Factory<T>::Factory(T* _pRoutineBox, const QJsonArray* _pRoutinesInfo) noexcept :
		pRoutineBox_(_pRoutineBox),
		pRoutinesInfo_(_pRoutinesInfo)
	{}

	template<RoutineBox T>
	Factory<T>::~Factory()
	{
		if (!pRoutineBox_ || !pRoutinesInfo_) { return; }

		for (auto valueRef : *pRoutinesInfo_) {
			if (valueRef.isObject()) {
				auto* pRoutine = create(valueRef.toObject());
				if (pRoutine) { pRoutineBox_->insert(pRoutineBox_->end(), pRoutine); }
			}
		}
	}

	template<RoutineBox T>
	Base* Factory<T>::create(const QJsonObject& _info) {
		if (!_info.contains(pact::routine::__typeKey__)) { return nullptr; }
		if (!_info.contains(pact::routine::__idKey__)) { return nullptr; }
		if (!_info.contains(pact::routine::__descriptionKey__)) { return nullptr; }

		QString type = _info.value(pact::routine::__typeKey__).toString();
		if (pact::routine::mock::__type__ == type) { return createMock(_info); }
		if (pact::routine::mbed::__type__ == type) { return createMbed(_info); }
		if (pact::routine::inner::osc::__type__ == type) { return createOscillation(_info); }
		if (pact::routine::udp::client::__type__ == type) { return createUdpClient(_info); }
		if (pact::routine::udp::server::__type__ == type) { return createUdpServer(_info); }

		return nullptr;
	}

	template<RoutineBox T>
	Base* Factory<T>::createUdpServer(const QJsonObject& _info) {
		QString uniqueName = _info.value(pact::routine::__idKey__).toString();
		QString description = _info.value(pact::routine::__descriptionKey__).toString();

		QString toggleID = _info.value(pact::routine::__toggleIDKey__).toString();
		QString statusID = _info.value(pact::routine::__statusIDKey__).toString();

		auto* pServer = new udp::Server(uniqueName, toggleID, statusID);
		pServer->description_ = description;

		pServer->hostName_ = _info.value(pact::routine::udp::__hostName__).toString();
		pServer->sendingPort_ = static_cast<quint16>(_info.value(pact::routine::udp::__sendingPort__).toInt());
		pServer->readingPort_ = static_cast<quint16>(_info.value(pact::routine::udp::__readingPort__).toInt());

		return pServer;
	}

	template<RoutineBox T>
	Base* Factory<T>::createUdpClient(const QJsonObject& _info) {
		QString uniqueName = _info.value(pact::routine::__idKey__).toString();
		QString description = _info.value(pact::routine::__descriptionKey__).toString();

		QString toggleID = _info.value(pact::routine::__toggleIDKey__).toString();
		QString statusID = _info.value(pact::routine::__statusIDKey__).toString();

		auto* pClient = new udp::Client(uniqueName, toggleID, statusID);
		pClient->description_ = description;

		QJsonArray prmIDs = _info.value(pact::routine::udp::client::__prmIDs__).toArray();
		for (auto valueRef : prmIDs) {
			QString prmID = valueRef.toString();
			auto* pPrm = Terminal::parameter(prmID);
			if (pPrm) { pClient->parameters_.push_back(pPrm); }
		}

		pClient->hostName_ = _info.value(pact::routine::udp::__hostName__).toString();
		pClient->sendingPort_ = static_cast<quint16>(_info.value(pact::routine::udp::__sendingPort__).toInt());
		pClient->readingPort_ = static_cast<quint16>(_info.value(pact::routine::udp::__readingPort__).toInt());

		return pClient;
	}

	template<RoutineBox T>
	Base* Factory<T>::createOscillation(const QJsonObject& _info) {
		QString uniqueName = _info.value(pact::routine::__idKey__).toString();
		QString description = _info.value(pact::routine::__descriptionKey__).toString();

		QString toggleID = _info.value(pact::routine::__toggleIDKey__).toString();
		QString statusID = _info.value(pact::routine::__statusIDKey__).toString();

		QString controlledID = _info.value(pact::routine::inner::osc::__controlledID__).toString();
		QString amplitudeID = _info.value(pact::routine::inner::osc::__amplitudeID__).toString();
		QString offsetID = _info.value(pact::routine::inner::osc::__offsetID__).toString();
		QString periodID = _info.value(pact::routine::inner::osc::__periodID__).toString();

		auto* pOsc = new inner::Oscillation(uniqueName, toggleID, statusID);
		pOsc->description_ = description;

		pOsc->pControlled_ = dynamic_cast<Parameter<qreal>*>(Terminal::parameter(controlledID));
		pOsc->pAmplitude_ = dynamic_cast<Parameter<qreal>*>(Terminal::parameter(amplitudeID));
		pOsc->pPeriod_ = dynamic_cast<Parameter<qreal>*>(Terminal::parameter(periodID));
		pOsc->pOffset_ = dynamic_cast<Parameter<qreal>*>(Terminal::parameter(offsetID));

		return pOsc;
	}

	template<RoutineBox T>
	Base* Factory<T>::createMbed(const QJsonObject& _info) {
		if (!_info.contains(pact::routine::mbed::__devicesArrayKey__)) { return nullptr; }
		if (!_info.value(pact::routine::mbed::__devicesArrayKey__).isArray()) { return nullptr; }

		QString uniqueName = _info.value(pact::routine::__idKey__).toString();
		QString description = _info.value(pact::routine::__descriptionKey__).toString();
		QString portName = _info.value(pact::routine::mbed::__portNameKey__).toString();

		QString toggleID = _info.value(pact::routine::__toggleIDKey__).toString();
		QString statusID = _info.value(pact::routine::__statusIDKey__).toString();

		auto* pServer = new mbed::Server(uniqueName, toggleID, statusID);
		pServer->description_ = description;
		pServer->portName_ = portName;

		{
			QJsonArray devices = _info.value(pact::routine::mbed::__devicesArrayKey__).toArray();
			mbed::device::Factory dvcFactory(&pServer->devices_, &devices);
		}

		return pServer;
	}

	template<RoutineBox T>
	Base* Factory<T>::createMock(const QJsonObject& _info) {
		if (!_info.contains(pact::routine::mock::__devicesArrayKey__)) { return nullptr; }

		QString uniqueName = _info.value(pact::routine::__idKey__).toString();
		QString description = _info.value(pact::routine::__descriptionKey__).toString();
		QString toggleID = _info.value(pact::routine::__toggleIDKey__).toString();
		QString statusID = _info.value(pact::routine::__statusIDKey__).toString();

		auto* pMock = new mock::Server(uniqueName, toggleID, statusID);
		pMock->description_ = description;

		QJsonArray devices = _info.value(pact::routine::mock::__devicesArrayKey__).toArray();
		for (auto valueRef : devices) {
			QJsonObject deviceObj = valueRef.toObject();

			QString type = deviceObj.value(pact::routine::mock::device::__typeKey__).toString();
			if (pact::routine::mock::device::led::__type__ == type) {
				QString stateReadID = deviceObj.value(pact::routine::mock::device::led::__stateReadID__).toString();
				QString stateEstID = deviceObj.value(pact::routine::mock::device::led::__stateEstID__).toString();

				auto* pStateReadPrm = dynamic_cast<Parameter<ToggleStatus>*>(Terminal::parameter(stateReadID));
				auto* pStateEstPrm = dynamic_cast<Parameter<bool>*>(Terminal::parameter(stateEstID));

				auto* pLED = new mock::device::LED(pStateReadPrm, pStateEstPrm);
				pMock->devices_.push_back(pLED);
			}
			else if (pact::routine::mock::device::osc::__type__ == type) {
				QString valueID = deviceObj.value(pact::routine::mock::device::osc::__valueID__).toString();
				QString amplitudeID = deviceObj.value(pact::routine::mock::device::osc::__amplitudeID__).toString();
				QString periodID = deviceObj.value(pact::routine::mock::device::osc::__periodID__).toString();

				auto* pValueReadPrm = dynamic_cast<Parameter<double>*>(Terminal::parameter(valueID));
				auto* pAmplitudePrm = dynamic_cast<Parameter<double>*>(Terminal::parameter(amplitudeID));
				auto* pPeriodPrm = dynamic_cast<Parameter<double>*>(Terminal::parameter(periodID));

				auto* pOsc = new mock::device::Oscillator(pValueReadPrm, pAmplitudePrm, pPeriodPrm);
				pMock->devices_.push_back(pOsc);
			}
		}

		return pMock;
	}
}