#pragma once
#include "gui/gui_i_readout.h"
#include <QWidget>

namespace gui::item {
	template<typename _T>
	concept ItemBox = requires(_T _obj) {
		_obj.end();
		_obj.insert(_obj.end(), *(_obj.begin()));
	};

	template<ItemBox _T>
	class Factory;

	class Base :
		public QWidget,
		public IReadout
	{
		Q_OBJECT

		friend class Factory<std::vector<Base*>>;

	public:
		const QString& uniqueName() const;

	public:
		explicit Base(const QString& _uniqueName, QWidget* _pParent = nullptr) :
			QWidget(_pParent),
			uniqueName_(_uniqueName)
		{}

		virtual ~Base() override = default;

	private:
		Base() = default;

	private:
		const QString uniqueName_{};
		QString description_{};

	protected:
		static const QString& onlineSignature();
		static const QString& offlineSignature();

		static const QColor& onlineColor();
		static const QColor& offlineColor();
		static const QColor& neutralColor();

	private:
		static const QString onlineSignature_;
		static const QString offlineSignature_;
		static const QColor onlineColor_;
		static const QColor offlineColor_;
		static const QColor neutralColor_;
	};

	inline const QString& Base::uniqueName() const {
		return uniqueName_;
	}

	inline const QString& Base::onlineSignature() {
		return onlineSignature_;
	}

	inline const QString& Base::offlineSignature() {
		return offlineSignature_;
	}
	
	inline const QColor& Base::onlineColor() {
		return onlineColor_;
	}

	inline const QColor& Base::offlineColor() {
		return offlineColor_;
	}

	inline const QColor& Base::neutralColor() {
		return neutralColor_;
	}
}
