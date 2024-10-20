#pragma once
#include <QByteArray>
#include "engine/engine_parameter.h"
#include "engine/engine_terminal.h"

namespace engine::routine::mbed {
	class Server;
}

namespace engine::routine::mbed::device {
	class Factory;

	class Base
	{
		friend class Server;
		friend class Factory;

	protected:
		virtual QByteArray request() const = 0;
		virtual bool receive(const QByteArray&) = 0;

	protected:
		uchar id() const;
		void setOnline(bool _isOnline);

	protected:
		virtual ~Base() noexcept = default;
		explicit Base(uchar _id, const QString& _feedbackStatusID) noexcept;

	private:
		static constexpr uchar INVALID_ID = 0x00;

	private:
		uchar id_{ INVALID_ID };
		Parameter<bool>* pFeedbackStatus_{ nullptr };
	};

	inline uchar Base::id() const {
		return id_;
	}

	inline Base::Base(uchar _id, const QString& _feedbackStatusID) noexcept :
		id_(_id), 
		pFeedbackStatus_(dynamic_cast<Parameter<bool>*>(Terminal::parameter(_feedbackStatusID)))
	{}

	inline void Base::setOnline(bool _isOnline) {
		if (pFeedbackStatus_) { pFeedbackStatus_->accept(&_isOnline); }
	}
}