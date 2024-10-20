#pragma once
// pact:
#include "gui/pact/gui_pact.h"

// items:
#include "gui/item/gui_item_toggle.h"
#include "gui/item/gui_item_mock.h"
#include "gui/item/gui_item_read_only_value.h"
#include "gui/item/gui_item_controlled_value.h"

// engine:
#include "engine/engine_terminal.h"
#include "engine/engine_parameter.h"

// Qt tools:
#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>

namespace gui::item {
	template<ItemBox Vec>
	class Factory
	{
	public:
		explicit Factory(Vec*, const QJsonArray*, QWidget*) noexcept;
		Factory(const Factory&) = delete;
		~Factory();

	private:
		Factory() noexcept = default;

	private:
		Vec* pItemBox_{ nullptr };
		const QJsonArray* pItemsInfo_{ nullptr };
		QWidget* pParent_{ nullptr };

	private:
		static Base* create(const QJsonObject&, QWidget*);

	private:
		static Base* createOscillator(const QJsonObject&, QWidget*);
		static Base* createToggle(const QJsonObject&, QWidget*);
		static Base* createReadOnlyValue(const QJsonObject&, QWidget*);
		static Base* createControlledValue(const QJsonObject&, QWidget*);
	};

	template<ItemBox Vec>
	Factory<Vec>::Factory(Vec* _pItemBox, const QJsonArray* _pItemsInfo, QWidget* _pParent) noexcept :
		pItemBox_(_pItemBox),
		pItemsInfo_(_pItemsInfo),
		pParent_(_pParent)
	{}

	template<ItemBox Vec>
	Factory<Vec>::~Factory() 
	{
		if (!pItemBox_ || !pItemsInfo_) { return; }

		for (auto valueRef : *pItemsInfo_) {
			auto* pItem = create(valueRef.toObject(), pParent_);
			if (pItem) { pItemBox_->insert(pItemBox_->end(), pItem); }
		}
	}

	template<ItemBox T>
	Base* Factory<T>::create(const QJsonObject& _info, QWidget* _pParent) {
		if (!_info.contains(pact::item::__typeKey__)) { return nullptr; }
		if (!_info.contains(pact::item::__uniqueNameKey__)) { return nullptr; }

		QString type = _info.value(pact::item::__typeKey__).toString();
		if (pact::item::osc::__typeSignature__ == type) { return createOscillator(_info, _pParent); }
		if (pact::item::toggle::__typeSignature__ == type) { return createToggle(_info, _pParent); }
		if (pact::item::readonly::__typeSignature__ == type) { return createReadOnlyValue(_info, _pParent); }
		if (pact::item::controlled::__typeSignature__ == type) { return createControlledValue(_info, _pParent); }

		return nullptr;
	}

	template<ItemBox T>
	Base* Factory<T>::createControlledValue(const QJsonObject& _info, QWidget* _pParent) {
		QString uniqueName = _info.value(pact::item::__uniqueNameKey__).toString();
		QString description = _info.value(pact::item::__descriptionKey__).toString();

		QString valueEstID = _info.value(pact::item::controlled::__valueEstIDKey__).toString();
		
		auto* pValueEstPrm = dynamic_cast<engine::Parameter<qreal>*>(engine::Terminal::parameter(valueEstID));
		if (!pValueEstPrm) { return nullptr; }

		auto* pControlledValueWidget = new ControlledValue(uniqueName, _pParent);
		pControlledValueWidget->description_ = description;

		pControlledValueWidget->pValueEst_ = pValueEstPrm;
		pControlledValueWidget->ui.labelValueName->setText(description);

		if (_info.contains(pact::item::controlled::__feedbackStatusID__)) {
			QString feedbackStatusID = _info.value(pact::item::controlled::__feedbackStatusID__).toString();
			auto* pFeedbackStatusPrm = dynamic_cast<engine::Parameter<bool>*>(engine::Terminal::parameter(feedbackStatusID));
			pControlledValueWidget->pFeedbackStatus_ = pFeedbackStatusPrm;
		}
		if (_info.contains(pact::item::controlled::__valueReadIDKey__)) {
			QString valueReadID = _info.value(pact::item::controlled::__valueReadIDKey__).toString();
			auto* pValueReadPrm = dynamic_cast<engine::Parameter<qreal>*>(engine::Terminal::parameter(valueReadID));
			pControlledValueWidget->pValueRead_ = pValueReadPrm;
		}

		return pControlledValueWidget;
	}

	template<ItemBox T>
	Base* Factory<T>::createReadOnlyValue(const QJsonObject& _info, QWidget* _pParent) {
		QString uniqueName = _info.value(pact::item::__uniqueNameKey__).toString();
		QString description = _info.value(pact::item::__descriptionKey__).toString();

		QString valueID = _info.value(pact::item::readonly::__valueIDKey__).toString();

		auto* pValuePrm = dynamic_cast<engine::Parameter<qreal>*>(engine::Terminal::parameter(valueID));
		if (!pValuePrm) { return nullptr; }

		auto* pReadOnlyWidget = new ReadOnlyValue(uniqueName, _pParent);
		pReadOnlyWidget->description_ = description;

		pReadOnlyWidget->pValue_ = pValuePrm;
		pReadOnlyWidget->ui.labelDescription->setText(pValuePrm->description());

		if (_info.contains(pact::item::readonly::__precisionKey__)) {
			pReadOnlyWidget->precision_ = _info.value(pact::item::readonly::__precisionKey__).toInt();
		}

		if (_info.contains(pact::item::readonly::__formatKey__)) {
			QString format = _info.value(pact::item::readonly::__formatKey__).toString();

			if (pact::item::readonly::__floating__ == format) { pReadOnlyWidget->format_ = ReadOnlyValue::Format::floating; }
			if (pact::item::readonly::__exponential__ == format) { pReadOnlyWidget->format_ = ReadOnlyValue::Format::exponential; }
		}

		if (_info.contains(pact::item::readonly::__feedbackStatusID__)) {
			QString feedbackStatusID = _info.value(pact::item::readonly::__feedbackStatusID__).toString();
			auto* pFeedbackStatusPrm = dynamic_cast<engine::Parameter<bool>*>(engine::Terminal::parameter(feedbackStatusID));
			pReadOnlyWidget->pFeedbackStatus_ = pFeedbackStatusPrm;
		}

		return pReadOnlyWidget;
	}

	template<ItemBox T>
	Base* Factory<T>::createOscillator(const QJsonObject& _info, QWidget* _pParent) {
		QString uniqueName = _info.value(pact::item::__uniqueNameKey__).toString();
		QString description = _info.value(pact::item::__descriptionKey__).toString();

		QString valueID = _info.value(pact::item::osc::__valuePrmID__).toString();
		QString amplitudeID = _info.value(pact::item::osc::__ampPrmID__).toString();
		QString periodID = _info.value(pact::item::osc::__periodPrmID__).toString();

		auto* pValuePrm = dynamic_cast<engine::Parameter<qreal>*>(engine::Terminal::parameter(valueID));
		if (!pValuePrm) { return nullptr; }

		auto* pAmplitudePrm = dynamic_cast<engine::Parameter<qreal>*>(engine::Terminal::parameter(amplitudeID));
		if (!pAmplitudePrm) { return nullptr; }

		auto* pPeriodPrm = dynamic_cast<engine::Parameter<qreal>*>(engine::Terminal::parameter(periodID));
		if (!pPeriodPrm) { return nullptr; }

		auto* pMockWidget = new Mock(uniqueName, _pParent);
		pMockWidget->description_ = description;

		pMockWidget->pValue_ = pValuePrm;
		pMockWidget->pAmplitude_ = pAmplitudePrm;
		pMockWidget->pPeriod_ = pPeriodPrm;

		pMockWidget->ui.groupBox->setTitle(pMockWidget->description_);

		return pMockWidget;
	}

	template<ItemBox T>
	Base* Factory<T>::createToggle(const QJsonObject& _info, QWidget* _pParent) {
		QString uniqueName = _info.value(pact::item::__uniqueNameKey__).toString();
		QString description = _info.value(pact::item::__descriptionKey__).toString();

		QString stateEstID = _info.value(pact::item::toggle::__stateEstID__).toString();

		auto* pStateEstPrm = dynamic_cast<engine::Parameter<bool>*>(engine::Terminal::parameter(stateEstID));
		if (!pStateEstPrm) { return nullptr; }

		auto* pToggleWidget = new Toggle(uniqueName, _pParent);
		pToggleWidget->description_ = description;

		pToggleWidget->pStateEst_ = pStateEstPrm;

		if (_info.contains(pact::item::toggle::__feedbackStatusID__)) {
			QString feedbackStatusID = _info.value(pact::item::toggle::__feedbackStatusID__).toString();
			auto* pFeedbackStatusPrm = dynamic_cast<engine::Parameter<bool>*>(engine::Terminal::parameter(feedbackStatusID));
			pToggleWidget->pFeedbackStatus_ = pFeedbackStatusPrm;
		}
		if (_info.contains(pact::item::toggle::__stateReadID__)) {
			QString stateReadID = _info.value(pact::item::toggle::__stateReadID__).toString();
			auto* pStateReadPrm = dynamic_cast<engine::Parameter<engine::ToggleStatus>*>(engine::Terminal::parameter(stateReadID));
			pToggleWidget->pStateRead_ = pStateReadPrm;
		}

		pToggleWidget->ui.checkBoxStateEst->setText(pToggleWidget->description_);

		return pToggleWidget;
	}
}
