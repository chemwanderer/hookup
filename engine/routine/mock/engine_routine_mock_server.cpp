#include "engine_routine_mock_server.h"

namespace engine::routine::mock {
	Server::Server(const QString& _uniqueName, const QString _togglePrmID, const QString& _currentStatusPrmID) noexcept :
		Base(_uniqueName, _togglePrmID, _currentStatusPrmID)
	{}

	Server::~Server() noexcept
	{
		for (auto* pDevice : devices_) {
			if (pDevice) { delete pDevice; }
		}
	}

	void Server::run() {
		if (devices_.empty()) { return; }
		itCurrentDevice_ = devices_.begin();

		while (true) {
			if (!checkToggle()) { return; }

			auto* pDevice = currentDevice();
			pDevice->handleParameters();
			nextDevice();
		}
	}

	void Server::beforeQuit() {}
}
