#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include "gui/item/gui_item_base.h"

namespace gui {
	class Terminal;

	class Block :
		public QWidget
	{
		Q_OBJECT

		friend class Terminal;

	public:
		const QString& blockTitle() const;

	public:
		virtual ~Block() override final = default;

	private:
		explicit Block(QWidget* _pParent = nullptr);

	private:
		void addItem(item::Base*);
		void setBlockTitle(const QString&);

	private:
		QVBoxLayout* pLayout_{ nullptr };
		QString blockTitle_{ QStringLiteral("Untitled")};
	};

	inline Block::Block(QWidget* _pParent) :
		QWidget(_pParent)
	{
		pLayout_ = new QVBoxLayout(this);
	}

	inline void Block::addItem(item::Base* _pItem) {
		if (_pItem) { pLayout_->addWidget(_pItem); }
	}

	inline void Block::setBlockTitle(const QString& _blockTitle) {
		blockTitle_ = _blockTitle;
	}

	inline const QString& Block::blockTitle() const {
		return blockTitle_;
	}
}
