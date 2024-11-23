#pragma once
#include "engine/engine_common.h"
#include "engine/routine/engine_routine_common.h"
#include "engine/pact/engine_pact.h"
#include "engine/engine_parameter.h"

#include <QJsonArray>
#include <QJsonObject>

namespace engine {
	template<Container _Vec>
	class PrmFactory
	{
	public:
		explicit PrmFactory(_Vec*, const QJsonArray*) noexcept;
		PrmFactory(const PrmFactory&) = delete;
		~PrmFactory();

	private:
		PrmFactory() noexcept = default;

	private:
		_Vec* pPrmBox_{ nullptr };
		const QJsonArray* pPrmInfo_{};

	private:
		static IValue* create(const QJsonObject&);

	private:
		template<typename _ValueType>
		static IValue* createPrm(const QJsonObject&);
		
		static IValue* createToggleStatus(const QJsonObject&);
		static IValue* createRoutineStatus(const QJsonObject&);
	};

	template<Container _Vec>
	PrmFactory<_Vec>::PrmFactory(_Vec* _pPrmBox, const QJsonArray* _pPrmInfo) noexcept :
		pPrmBox_(_pPrmBox),
		pPrmInfo_(_pPrmInfo)
	{}

	template<Container _Vec>
	PrmFactory<_Vec>::~PrmFactory()
	{
		if (!pPrmBox_ || !pPrmInfo_) { return; }

		for (auto valueRef : *pPrmInfo_) {
			if (valueRef.isObject()) {
				auto* pDevice = create(valueRef.toObject());
				if (pDevice) { pPrmBox_->insert(pPrmBox_->end(), pDevice); }
			}
		}
	}

	template<Container _Vec>
	IValue* PrmFactory<_Vec>::create(const QJsonObject& _info) {
		if (!_info.contains(pact::parameter::__typeKey__)) { return nullptr; }
		if (!_info.contains(pact::parameter::__idKey__)) { return nullptr; }
		if (!_info.contains(pact::parameter::__descriptionKey__)) { return nullptr; }

		QString type = _info.value(pact::parameter::__typeKey__).toString();
		if (pact::parameter::type::__toggleStatusType__ == type) { return createToggleStatus(_info); }
		if (pact::parameter::type::__booleanType__ == type) { return createPrm<bool>(_info); }
		if (pact::parameter::type::__realNumberType__ == type) { return createPrm<qreal>(_info); }
		if (pact::parameter::type::__integerType__ == type) { return createPrm<int>(_info); }
		if (pact::parameter::type::__routineStatusType__ == type) { return createRoutineStatus(_info); }

		return nullptr;
	}

	template<Container _Vec>
	template<typename _ValueType>
	IValue* PrmFactory<_Vec>::createPrm(const QJsonObject& _info) {
		QString uniqueName = _info.value(pact::parameter::__idKey__).toString();
		QString description = _info.value(pact::parameter::__descriptionKey__).toString();
		bool isManual = _info.value(pact::parameter::__isManual__).toBool();

		auto* pPrm = new Parameter<_ValueType>(uniqueName, 0);
		pPrm->description_ = description;
		pPrm->isManual_.store(isManual);

		if (_info.contains(pact::parameter::__quantityNameKey__)) {
			pPrm->quantityName_ = _info.value(pact::parameter::__quantityNameKey__).toString();
		}
		if (_info.contains(pact::parameter::__unitsKey__)) {
			pPrm->units_ = _info.value(pact::parameter::__unitsKey__).toString();
		}

		return pPrm;
	}

	template<Container _T>
	IValue* PrmFactory<_T>::createToggleStatus(const QJsonObject& _info) {
		QString uniqueName = _info.value(pact::parameter::__idKey__).toString();
		QString description = _info.value(pact::parameter::__descriptionKey__).toString();

		auto* pToggleStatus = new Parameter<ToggleStatus>(uniqueName, ToggleStatus::undefined);
		pToggleStatus->description_ = description;
		pToggleStatus->isManual_.store(false);

		return pToggleStatus;
	}

	template<Container _T>
	IValue* PrmFactory<_T>::createRoutineStatus(const QJsonObject& _info) {
		QString uniqueName = _info.value(pact::parameter::__idKey__).toString();
		QString description = _info.value(pact::parameter::__descriptionKey__).toString();

		auto* pToggleStatus = new Parameter<routine::Status>(uniqueName, routine::Status::quitted);
		pToggleStatus->description_ = description;
		pToggleStatus->isManual_.store(false);

		return pToggleStatus;
	}
}
