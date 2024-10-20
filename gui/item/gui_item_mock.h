#pragma once

#include <QWidget>
#include "ui_gui_item_mock.h"

#include "gui/item/gui_item_base.h"
#include "engine/engine_parameter.h"

namespace gui::item {
	template<ItemBox T>
	class Factory;

	class Mock :
		public Base
	{
		Q_OBJECT

		friend class Factory<std::vector<Base*>>;

	public:
		virtual ~Mock() override final;

	private:
		explicit Mock(const QString& _uniqueName, QWidget* _pParent = nullptr);

	private:
		virtual void updateReadouts() override final;

	private:
		Ui::MockClass ui;
		engine::Parameter<qreal>* pAmplitude_{ nullptr };
		engine::Parameter<qreal>* pPeriod_{ nullptr };
		engine::Parameter<qreal>* pValue_{ nullptr };

	private slots:
		void slot_setPeriod();
		void slot_setAmplitude();
	};
}
