#include "engine_routine_base.h"

namespace engine::routine {
	Base::Base(const QString& _uniqueName, const QString _togglePrmID, const QString& _currentStatusPrmID) noexcept :
		uniqueName_(_uniqueName),
		pToggle_(dynamic_cast<Parameter<bool>*>(Terminal::parameter(_togglePrmID))),
		pCurrentStatus_(dynamic_cast<Parameter<Status>*>(Terminal::parameter(_currentStatusPrmID)))
	{
		if (pToggle_) {
			bool toggle = false;
			pToggle_->accept(&toggle);
		}
		if (pCurrentStatus_) {
			Status status = Status::quitted;
			pCurrentStatus_->accept(&status);
		}
	}

	Base::~Base() noexcept {
		if (pWorker_) {
			if (pWorker_->joinable()) { pWorker_->join(); }
			delete pWorker_;
		}
	}

	void Base::start() {
		if (!pToggle_ || !pCurrentStatus_) { return; }

		auto currentStatus = status();
		switch (currentStatus) {
		case Status::quitted: {
			break;
		}
		default: {
			return;
		}
		}

		setQuitFlag<false>();
		setStatus(Status::trigger);

		if (pWorker_) {
			if (pWorker_->joinable()) { pWorker_->join(); }
			delete pWorker_;
			pWorker_ = nullptr;
		}

		pWorker_ = new std::thread([this]()->void { exec(); });
	}

	void Base::brake() {
		if (!pToggle_ || !pCurrentStatus_) { return; }

		auto currentStatus = status();
		if (Status::quitted == currentStatus) { return; }

		setQuitFlag<true>();

		bool toggle = false;
		pToggle_->accept(&toggle);
	}

	void Base::exec() {
		bool quit = false;
		std::chrono::milliseconds nap(DEFAULT_SLEEP_DURATION);

		while (!quit) {
			setStatus(Status::waiting);

			quit = checkQuit();
			bool toggle = false;
			pToggle_->submit(&toggle);

			while (!(toggle || quit)) {
				pToggle_->submit(&toggle);
				quit = checkQuit();
				std::this_thread::sleep_for(nap);
			}

			if (quit) { break; }

			setStatus(Status::running);
			run();
			beforeQuit();
		}

		setStatus(Status::quitted);
	}
}