#pragma once
#include <QPointF>
#include <QTime>
#include <QFile>
#include <QElapsedTimer>
#include "engine/routine/engine_routine_base.h"
#include "kit/kit.h"

namespace engine::routine {
	template<RoutineBox _T>
	class Factory;
}

namespace engine::routine::inner {
	class Kinetics :
		public Base
	{
		friend class Factory<std::vector<Base*>>;

	public:
		virtual ~Kinetics() noexcept override final = default;
		Kinetics(const Kinetics&) = delete;

	private:
		explicit Kinetics(const QString& _uniqueName,
			const QString _togglePrmID, const QString& _currentStatusPrmID) noexcept;

	private:
		virtual void run() override final;
		virtual void beforeQuit() override final;

	private:
		static constexpr QStringView __outputDir__{ u"C:/hookup_data/curves/" };
		static constexpr uint32_t DEFAULT_PERIOD{ 100 };

	private:
		const Parameter<double>* pCurrentADC_{ nullptr };
		const Parameter<double>* pFullTransmittanceADC_{ nullptr };
		Parameter<double>* pTransmittance_{ nullptr };

		const Parameter<double>* pRecordTime_{ nullptr };

		uint32_t period_{ DEFAULT_PERIOD }; // ms
		std::vector<QPointF> curvePoints_{};
	};

	inline Kinetics::Kinetics(const QString& _uniqueName,
		const QString _togglePrmID, const QString& _currentStatusPrmID) noexcept :
		Base(_uniqueName, _togglePrmID, _currentStatusPrmID)
	{}
}
