#include "engine/routine/udp/engine_routine_udp_client.h"
#include "engine/routine/udp/engine_routine_udp_util.h"

namespace engine::routine::udp {
	void Client::run() {
		if (parameters_.empty()) { return; }
		itCurrent_ = parameters_.begin();

		pReadingSocket_ = new QUdpSocket;
		pSendingSocket_ = new QUdpSocket;

		pReadingSocket_->bind(QHostAddress::Any, readingPort_);
		pSendingSocket_->connectToHost(hostName_, sendingPort_);
		if (!pSendingSocket_->waitForConnected(5000)) { return; }

		while (true) {
			if (!checkToggle()) { return; }
			iteration();
			next();
		}
	}

	void Client::beforeQuit() {
		if (!pReadingSocket_ || !pSendingSocket_) { return; }

		pReadingSocket_->abort();
		pSendingSocket_->abort();

		delete pReadingSocket_;
		pReadingSocket_ = nullptr;

		delete pSendingSocket_;
		pSendingSocket_ = nullptr;
	}

	bool Client::getResponse() {
		quint32 attempt = 0;
		QByteArray res{};

		char nextRWFlag = -1;
		QString nextId{};
		QByteArray nextValue{};
		bool nextManualFlag = false;

		while (attempt < MAX_ATTEMPTS) {
			++attempt;
			if (!checkToggle()) { return false; }
			if (pReadingSocket_->waitForReadyRead(ATTEMPT_DURATION)) {
				QByteArray nextChunk(pReadingSocket_->pendingDatagramSize(), 0x00);
				pReadingSocket_->readDatagram(nextChunk.data(), nextChunk.size());
				res.append(nextChunk);

				if (util::parseMessage(res, lastRWFlag_, lastId_, lastValue_, lastManualFlag_)) { return true; }
			}
		}

		return false;
	}

	void Client::sendReadRequest() {
		auto* pCurrent = current();
		pSendingSocket_->write(util::message<util::READ_FLAG>(pCurrent->uniqueName(), QByteArray(), false));
	}

	void Client::sendWriteRequest() {
		auto* pCurrent = current();
		pSendingSocket_->write(util::message<util::WRITE_FLAG>(pCurrent->uniqueName(),
			pCurrent->submit(), pCurrent->manualControl()));
	}

	void Client::iteration() {
		sendReadRequest();
		if (!getResponse()) { return; }

		if (!lastManualFlag_) {
			updateCurrent();
			return;
		}

		current()->setManualControl(lastManualFlag_);
		sendWriteRequest();
		if (!getResponse()) { return; }
	}

	void Client::updateCurrent() {
		auto* pCurrent = current();
		if (pCurrent->uniqueName() == lastId_) {
			pCurrent->setManualControl(lastManualFlag_);
			pCurrent->accept(lastValue_.data());
		}
	}
}