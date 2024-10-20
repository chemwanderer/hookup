#pragma once
#include <QTimer>
#include "gui/gui_main_window.h"
#include "gui/item/gui_item_base.h"

namespace gui {
	class Terminal :
		public QObject
	{
		Q_OBJECT

	public:
		static Terminal* terminal();
		static bool build();
		static void launch();
		static item::Base* item(const QString& _uniqueName);

	public:
		virtual ~Terminal() override final;

	private:
		explicit Terminal(QObject* _pParent = nullptr);

	private:
		static Terminal* pTerminal_;
		static constexpr qint32 ITEMS_UPDATE_PERIOD = 5'00; // milliseconds

	private:
		std::vector<item::Base*> items_{};
		bool isBuilt_{ false };

		QTimer* pUpdateTimer_{ nullptr };
		MainWindow mainWindow_;

	private slots:
		void slot_updateItems();
	};

	inline Terminal* Terminal::terminal() {
		if (!pTerminal_) { pTerminal_ = new Terminal; }
		return pTerminal_;
	}
}
