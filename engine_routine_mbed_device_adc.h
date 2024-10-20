#pragma once
#include "engine/routine/mbed/device/engine_routine_mbed_device_base.h"
#include "engine/engine_parameter.h"
#include <array>

namespace engine::routine::mbed::device {
	class Factory;

	class ADC :
		public Base
	{
		friend class Factory;

	private:
		static constexpr quint32 CHANNELS_NUMBER = 6;
		static constexpr quint32 READ_ALL_RESPONSE_PACKAGE_SIZE = 13;
		static constexpr qreal INV_MAX_USHORT = 1. / 0xff'ff;
		static constexpr char READ_ALL_COMMAND = 0x01;
		static constexpr char READ_CHANNEL_COMMAND = 0x02;

	private:
		virtual QByteArray request() const override final;
		virtual bool receive(const QByteArray&) override final;

	public:
		virtual ~ADC() noexcept override final = default;

	private:
		explicit ADC(uchar _id, const QString& _feedbackStatusID) noexcept;

	private:
		std::array<Parameter<qreal>*, CHANNELS_NUMBER> channels_{
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		};
	};

	inline ADC::ADC(uchar _id, const QString& _feedbackStatusID) noexcept :
		Base(_id, _feedbackStatusID)
	{}
}