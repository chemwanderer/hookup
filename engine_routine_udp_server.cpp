#include "engine_routine_udp_server.h"
#include "engine/engine_terminal.h"
#include "engine/routine/udp/engine_routine_udp_util.h"

namespace engine::routine::udp {
	void Server::run() {
		pReadingSocket_ = new QUdpSocket;
		pSendingSocket_ = new QUdpSocket;

		pReadingSocket_->bind(QHostAddress::Any, readingPort_);
		pSendingSocket_->connectToHost(hostName_, sendingPort_);
		if (!pSendingSocket_->waitForConnected(5000)) { return; }

		while (true) {
			if (!checkToggle()) { return; }

			if (!waitForRequest()) { return; }
			
			auto* pCurrent = Terminal::parameter(lastId_);
			if (!pCurrent) { continue; }

			QByteArray response{};
			switch (lastRWFlag_) {
			case util::WRITE_FLAG: {
				pCurrent->accept(lastValue_.data());
				response = util::message<util::WRITE_FLAG>(pCurrent->uniqueName(), 
					pCurrent->submit(), pCurrent->manualControl());
				break;
			}
			case util::READ_FLAG: {
				response = util::message<util::READ_FLAG>(pCurrent->uniqueName(),
					pCurrent->submit(), pCurrent->manualControl());
				break;
			}
			}

			pSendingSocket_->write(response);
		}
	}

	void Server::beforeQuit() {
		pReadingSocket_->abort();
		pSendingSocket_->abort();

		delete pReadingSocket_;
		pReadingSocket_ = nullptr;

		delete pSendingSocket_;
		pSendingSocket_ = nullptr;
	}

	bool Server::waitForRequest() {
		QByteArray request{};
		while (true) {
			if (!checkToggle()) { return false; }
			if (pReadingSocket_->waitForReadyRead(ATTEMPT_DURATION)) {
				QByteArray nextChunk(pReadingSocket_->pendingDatagramSize(), 0x00);
				pReadingSocket_->readDatagram(nextChunk.data(), nextChunk.size());
				request.append(nextChunk);

				if (util::parseMessage(request, lastRWFlag_, lastId_, lastValue_, lastManualFlag_)) { return true; }
			}
		}

		return false;
	}
}