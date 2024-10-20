#pragma once
#include "engine/routine/mbed/device/engine_routine_mbed_device_base.h"
#include "engine/engine_parameter.h"

namespace engine::routine::mbed::device {
	class Factory;

	class Toggle :
		public Base
	{
		friend class Factory;

	private:
		virtual QByteArray request() const override final;
		virtual bool receive(const QByteArray&) override final;

	public:
		virtual ~Toggle() noexcept override final = default;

	private:
		explicit Toggle(uchar _id, const QString& _feedbackStatusID) noexcept;

	private:
		Parameter<ToggleStatus>* pStateRead_{ nullptr };
		Parameter<bool>* pIsOn_{ nullptr };

		mutable bool prevState_{ false };

	private:
		static constexpr char READ_STATE_COMMAND = 0x01;
		static constexpr char SET_STATE_COMMAND = 0x02;

		static constexpr quint32 RECEIVED_PACKAGE_SIZE = 2;
	};

	inline Toggle::Toggle(uchar _id, const QString& _feedbackStatusID) noexcept :
		Base(_id, _feedbackStatusID)
	{}
}