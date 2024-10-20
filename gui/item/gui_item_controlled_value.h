#pragma once

#include <QWidget>
#include "ui_gui_item_controlled_value.h"

#include "gui/item/gui_item_base.h"
#include "engine/engine_parameter.h"

namespace gui::item {
	template<ItemBox T>
	class Factory;

	class ControlledValue :
		public Base
	{
		Q_OBJECT

		friend class Factory<std::vector<Base*>>;

	public:
		virtual ~ControlledValue() override final;

	private:
		explicit ControlledValue(const QString&, QWidget* _pParent = nullptr);

	private:
		virtual void updateReadouts() override final;

	private:
		Ui::ControlledValueClass ui;
		int precision_{ 3 };

		const engine::Parameter<bool>* pFeedbackStatus_{ nullptr };
		const engine::Parameter<qreal>* pValueRead_{ nullptr };
		engine::Parameter<qreal>* pValueEst_{ nullptr };

	private slots:
		void slot_setValue();
	};
}
