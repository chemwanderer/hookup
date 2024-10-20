#include "gui_terminal.h"
#include "gui/item/gui_item_factory.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

namespace gui {
	Terminal* Terminal::pTerminal_ = nullptr;

	Terminal::Terminal(QObject* _pParent)
	{}

	Terminal::~Terminal()
	{}

	item::Base* Terminal::item(const QString& _uniqueName) {
		auto* pTerminal = terminal();

		auto& rItems = pTerminal->items_;
		auto itemIt = std::find_if(rItems.begin(), rItems.end(),
			[_uniqueName](item::Base* _pItem)->bool { return _pItem->uniqueName() == _uniqueName; });

		return rItems.end() == itemIt ? nullptr : *itemIt;
	}

	bool Terminal::build() {
		auto* pTerminal = terminal();
		if (pTerminal->isBuilt_) { return false; }

		QFile guiFile;
		guiFile.setFileName(pact::__guiPath__.toString());
		if (!guiFile.open(QFile::ReadOnly)) { return false; }

		QByteArray guiBytes = guiFile.readAll();

		QJsonParseError error(QJsonParseError::NoError);
		QJsonDocument guiDoc = QJsonDocument::fromJson(guiBytes, &error);
		if (error.error != QJsonParseError::NoError) { return false; }

		const QJsonObject guiObj = guiDoc.object();
		auto* pMainWindow = &pTerminal->mainWindow_;
		pMainWindow->setWindowTitle(pact::__mainWindowTitle__.toString());

		// ***
		// items building:
		{
			const QJsonArray itemsArray = guiObj.value(pact::__widgetsKey__).toArray();
			gui::item::Factory<std::vector<item::Base*>> itemBuilder(&pTerminal->items_, 
				&itemsArray, pMainWindow);
		}
		// - items building
		// ***

		// ***
		// blocks building & filling:
		{
			const QJsonArray blocksArray = guiObj.value(pact::__blocksKey__).toArray();
			for (auto blockValueRef : blocksArray) {
				if (!blockValueRef.isObject()) { continue; }

				auto obj = blockValueRef.toObject();
				QString title = obj.value(pact::block::__titleKey__).toString();

				auto widgetsArray = obj.value(pact::block::__widgets__).toArray();
				auto* pBlock = new Block(pMainWindow);
				pBlock->setBlockTitle(title);
				pMainWindow->addBlock(pBlock);

				for (auto widgetIDValueRef : widgetsArray) {
					if (!widgetIDValueRef.isString()) { continue; }

					auto* pItem = item(widgetIDValueRef.toString());
					if (!pItem) { continue; }

					pBlock->addItem(pItem);
				}
			}
		}
		// - blocks building & filling
		// ***

		pMainWindow->show();

		pTerminal->pUpdateTimer_ = new QTimer(pTerminal);
		connect(pTerminal->pUpdateTimer_, &QTimer::timeout, pTerminal, &Terminal::slot_updateItems);

		pTerminal->isBuilt_ = true;

		return true;
	}

	void Terminal::launch() {
		auto* pTerminal = terminal();
		if (!pTerminal->isBuilt_) { return; }

		auto* pTimer = pTerminal->pUpdateTimer_;
		pTimer->setInterval(Terminal::ITEMS_UPDATE_PERIOD);
		pTimer->start();
	}

	void Terminal::slot_updateItems() {
		for (auto* pItem : items_) {
			pItem->updateReadouts();
		}
	}
}
