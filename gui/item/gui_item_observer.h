#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QTime>
#include "ui_gui_item_observer.h"

#include "gui/item/gui_item_base.h"
#include "engine/engine_parameter.h"
#include "chart/chart_model.h"
#include "kit/kit.h"

namespace gui::item {
	template<ItemBox T>
	class Factory;

	class Observer :
		public Base
	{
		Q_OBJECT

		friend class Factory<std::vector<Base*>>;

	public:
		virtual ~Observer() override final;

	private:
		explicit Observer(const QString&, QWidget* _pParent = nullptr);

	private:
		virtual void updateReadouts() override final;

	private:
		Ui::ObserverClass ui;

		const engine::Parameter<qreal>* pValueRead_{ nullptr };
		const engine::Parameter<bool>* pFeedbackStatus_{ nullptr };

		QTimer* pCaptureTimer_{ nullptr };
		QTime startTime_;
		int capturePeriod_{ 200 };
		bool isRunning_{ false };

		qreal currentMinValue_{ 0. };
		qreal currentMaxValue_{ 1. };

		std::vector<QPointF> capturedPoints_{};
		chart::Model<std::vector<QPointF>, QRectF> model_;

	private slots:
		void slot_capture();
		void slot_switch(bool);
		void slot_setCapturePeriod();
		void slot_loadCurve();
	};
}
