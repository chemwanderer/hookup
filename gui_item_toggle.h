#pragma once

#include <QWidget>
#include "ui_gui_item_toggle.h"

#include "gui/item/gui_item_base.h"
#include "engine/engine_parameter.h"

namespace gui::item {
	template<ItemBox T>
	class Factory;

	class Toggle : 
		public Base
	{
		Q_OBJECT

		friend class Factory<std::vector<Base*>>;

	public:
		virtual ~Toggle() override final;

	private:
		explicit Toggle(const QString&, QWidget* _pParent = nullptr);

	private:
		virtual void updateReadouts() override final;

	private:
		Ui::ToggleClass ui;

		engine::Parameter<bool>* pStateEst_{ nullptr };
		const engine::Parameter<engine::ToggleStatus>* pStateRead_{ nullptr };
		const engine::Parameter<bool>* pFeedbackStatus_{ nullptr };

	private slots:
		void slot_setState(bool);
	};
}
