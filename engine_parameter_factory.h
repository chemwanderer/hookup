#pragma once
#include "engine/engine_common.h"
#include "engine/routine/engine_routine_common.h"
#include "engine/pact/engine_pact.h"
#include "engine/engine_parameter.h"

#include <QJsonArray>
#include <QJsonObject>

namespace engine {
	template<Container Vec>
	class PrmFactory
	{
	public:
		explicit PrmFactory(Vec*, const QJsonArray*) noexcept;
		PrmFactory(const PrmFactory&) = delete;
		~PrmFactory();

	private:
		PrmFactory() noexcept = default;

	private:
		Vec* pPrmBox_{ nullptr };
		const QJsonArray* pPrmInfo_{};

	private:
		static IValue* create(const QJsonObject&);

	private:
		template<typename ValueType>
		static IValue* createPrm(const QJsonObject&);
		
		static IValue* createToggleStatus(const QJsonObject&);
		static IValue* createRoutineStatus(const QJsonObject&);
	};

	template<Container Vec>
	PrmFactory<Vec>::PrmFactory(Vec* _pPrmBox, const QJsonArray* _pPrmInfo) noexcept :
		pPrmBox_(_pPrmBox),
		pPrmInfo_(_pPrmInfo)
	{}

	template<Container Vec>
	PrmFactory<Vec>::~PrmFactory()
	{
		if (!pPrmBox_ || !pPrmInfo_) { return; }

		for (auto valueRef : *pPrmInfo_) {
			if (valueRef.isObject()) {
				auto* pDevice = create(valueRef.toObject());
				if (pDevice) { pPrmBox_->insert(pPrmBox_->end(), pDevice); }
			}
		}
	}

	template<Container Vec>
	IValue* PrmFactory<Vec>::create(const QJsonObject& _info) {
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

	template<Container Vec> 
	template<typename ValueType>
	IValue* PrmFactory<Vec>::createPrm(const QJsonObject& _info) {
		QString uniqueName = _info.value(pact::parameter::__idKey__).toString();
		QString description = _info.value(pact::parameter::__descriptionKey__).toString();
		bool isManual = _info.value(pact::parameter::__isManual__).toBool();

		auto* pPrm = new Parameter<ValueType>(uniqueName, 0);
		pPrm->description_ = description;
		pPrm->isManual_.store(isManual);

		return pPrm;
	}

	template<Container T>
	IValue* PrmFactory<T>::createToggleStatus(const QJsonObject& _info) {
		QString uniqueName = _info.value(pact::parameter::__idKey__).toString();
		QString description = _info.value(pact::parameter::__descriptionKey__).toString();

		auto* pToggleStatus = new Parameter<ToggleStatus>(uniqueName, ToggleStatus::undefined);
		pToggleStatus->description_ = description;
		pToggleStatus->isManual_.store(false);

		return pToggleStatus;
	}

	template<Container T>
	IValue* PrmFactory<T>::createRoutineStatus(const QJsonObject& _info) {
		QString uniqueName = _info.value(pact::parameter::__idKey__).toString();
		QString description = _info.value(pact::parameter::__descriptionKey__).toString();

		auto* pToggleStatus = new Parameter<routine::Status>(uniqueName, routine::Status::quitted);
		pToggleStatus->description_ = description;
		pToggleStatus->isManual_.store(false);

		return pToggleStatus;
	}
}