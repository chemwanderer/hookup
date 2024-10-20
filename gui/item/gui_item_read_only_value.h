#pragma once

#include <QWidget>
#include "ui_gui_item_read_only_value.h"

#include "gui/item/gui_item_base.h"
#include "engine/engine_parameter.h"

namespace gui::item {
	template<ItemBox T>
	class Factory;

	class ReadOnlyValue : 
		public Base
	{
		Q_OBJECT

		friend class Factory<std::vector<Base*>>;

	private:
		enum class Format : uchar {
			floating,
			exponential
		};

	public:
		virtual ~ReadOnlyValue() override final;

	private:
		explicit ReadOnlyValue(const QString&, QWidget* _pParent = nullptr);

	private:
		virtual void updateReadouts() override final;

	private:
		Ui::ReadOnlyValueClass ui;

		Format format_{ Format::floating };
		int precision_{ 3 };

		const engine::Parameter<qreal>* pValue_{ nullptr };
		const engine::Parameter<bool>* pFeedbackStatus_{ nullptr };
	};
}
