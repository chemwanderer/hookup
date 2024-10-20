#pragma once
//////////////////////////////////////////////////////
//													//
// Base class of a separate-threaded routine.		//
//													//
//////////////////////////////////////////////////////
#include <mutex>

#include "engine/routine/engine_routine_common.h"
#include "engine/pact/engine_pact.h"
#include "engine/engine_parameter.h"
#include "engine/engine_terminal.h"

namespace engine::routine {
	template<RoutineBox T>
	class Factory;

	class Base
	{
		friend class Factory<std::vector<Base*>>;
		friend void Terminal::launch();
		friend void Terminal::halt();

	// ***
	// methods for routine management:
	public:
		void start();
		void brake();
		Status status() const;

		const QString& uniqueName() const;

	protected:
		bool checkToggle() const;
	// - methods for routine management
	// ***

	public:
		virtual ~Base() noexcept;

	protected:
		explicit Base(const QString& _uniqueName, const QString _togglePrmID, const QString& _currentStatusPrmID) noexcept;

	private:
		Base() noexcept { assert(false); }

	protected:
		virtual void run() = 0;
		virtual void beforeQuit();

	private:
		void exec();
		void setStatus(Status _status);
		bool checkQuit() const;

		template<bool _quitFlag>
		void setQuitFlag();

	private:
		// internal/unchangable parameters:
		const QString uniqueName_{};
		QString description_{};

		std::thread* pWorker_{ nullptr };
		// - internal/unchangable parameters

		// shared parameters:
		std::atomic<bool> quitFlag_{ false };
		
		Parameter<Status>* pCurrentStatus_{ nullptr };
		Parameter<bool>* pToggle_{ nullptr };
		// - shared parameters
	private:
		static constexpr quint32 DEFAULT_SLEEP_DURATION = 100; // ms
	};

	inline void Base::beforeQuit() {}

	inline bool Base::checkToggle() const {
		bool toggle = false;
		pToggle_->submit(&toggle);
		return toggle;
	}

	inline Status Base::status() const {
		Status res = Status::quitted;
		pCurrentStatus_->submit(&res);
		return res;
	}

	inline const QString& Base::uniqueName() const {
		return uniqueName_;
	}

	inline void Base::setStatus(Status _status) {
		pCurrentStatus_->accept(&_status);
	}

	inline bool Base::checkQuit() const {
		return quitFlag_.load();
	}

	template<bool _quitFlag>
	inline void Base::setQuitFlag() {
		quitFlag_.store(_quitFlag);
	}
}