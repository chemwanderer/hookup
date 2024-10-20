#pragma once
#include "engine/routine/mbed/device/engine_routine_mbed_device_base.h"
#include "engine/engine_parameter.h"

namespace engine::routine::mbed::device {
	class Factory;

	class MCP4725 :
		public Base
	{
		friend class Factory;

	private:
		static constexpr char CHECK_COMMAND = 0x01;
		static constexpr char SET_VALUE_COMMAND = 0x02;
		static constexpr quint16 DAC_AMPLITUDE = 0x10'00;
		static constexpr qreal ZERO_THRESHOLD = 1. / 0x10'00;

	private:
		virtual QByteArray request() const override final;
		virtual bool receive(const QByteArray&) override final;

	public:
		virtual ~MCP4725() noexcept override final = default;

	private:
		explicit MCP4725(uchar _id, const QString& _feedbackStatusID) noexcept;

	private:
		Parameter<qreal>* pValueEst_{ nullptr };	// from 0.0 to 1.0
		mutable qreal prevValueEst_{ 0. };
	};

	inline MCP4725::MCP4725(uchar _id, const QString& _feedbackStatusID) noexcept :
		Base(_id, _feedbackStatusID)
	{}
}