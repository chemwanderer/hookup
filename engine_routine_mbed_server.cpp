#include "engine_routine_mbed_server.h"

namespace engine::routine::mbed {
	Server::~Server() noexcept {
		for (auto* pDevice : devices_) {
			if (pDevice) { delete pDevice; }
		}
	}

	void Server::setupSerialPort() {
		pSerialPort_->setPortName(portName_);
		pSerialPort_->setBaudRate(BAUD_RATE, QSerialPort::AllDirections);
		pSerialPort_->setParity(QSerialPort::NoParity);
		pSerialPort_->setDataBits(QSerialPort::Data8);
		pSerialPort_->setStopBits(QSerialPort::OneStop);
		pSerialPort_->setFlowControl(QSerialPort::NoFlowControl);
	}

	void Server::writeChecksum(char* _pDst, quint32 _size) {
		char res = 0x00;
		for (quint32 i = 0; i + 1 < _size; ++i) {
			res ^= *_pDst;
			++_pDst;
		}
		*_pDst = res;
	}

	bool Server::verifyChecksum(const char* _pSrc, quint32 _size) {
		char res = 0x00;
		for (quint32 i = 0; i + 1 < _size; ++i) {
			res ^= *_pSrc;
			++_pSrc;
		}
		return !std::memcmp(&res, _pSrc, sizeof(char));
	}

	bool Server::readResponse(quint32 _deviceIdx) {
		QByteArray wrapped{};
		QByteArray unwrapped{};

		bool ok = false;
		uchar deviceID = device::Base::INVALID_ID;

		quint32 attempt = 0;
		while (attempt < MAX_READ_ATTEMPTS_NUMBER) {
			++attempt;

			if (pSerialPort_->waitForReadyRead(static_cast<int>(READ_ATTEMPT_DURATION))) {
				attempt = 0;
				QByteArray next = pSerialPort_->readAll();
				wrapped.append(next);

				unwrapped = unwrap(wrapped, &deviceID, &ok);
				if (ok) {
					attempt = 0;
					responses_.push_back(std::make_tuple(_deviceIdx, deviceID, unwrapped));
					unwrapped.detach();
					break;
				}
			}
		}

		return ok;
	}

	bool Server::processResponse() {
		if (responses_.empty()) { return false; }

		auto& rFirst = responses_.front();
		auto* pDevice = devices_[std::get<1>(rFirst)];
		bool result = false;

		if (std::get<1>(rFirst) == pDevice->id()) { 
			result = pDevice->receive(std::get<2>(rFirst)); 
			pDevice->setOnline(result);
		}
		responses_.pop_front();

		return result;
	}

	void Server::processAllResponses() {
		for (auto& rResp : responses_) {
			auto* pDevice = devices_[std::get<0>(rResp)];
			if (std::get<1>(rResp) == pDevice->id()) { 
				bool ok = pDevice->receive(std::get<2>(rResp));
				pDevice->setOnline(ok);
			}
		}
		responses_.clear();
	}

	device::Base* Server::device(uchar _id) {
		auto itDevice = std::find_if(devices_.begin(), devices_.end(),
			[_id](device::Base* _pDevice)->bool { return _id == _pDevice->id(); });
		
		if (devices_.end() != itDevice) { return *itDevice; }
		return nullptr;
	}

	QByteArray Server::wrap(const QByteArray& _package, uchar _deviceID) {
		QByteArray res(_package.size() + WRAPPER_SIZE, 0x00);
		char* pPackage = res.data();
		char* pSlider = pPackage;

		*pSlider = HEADER_BYTE;
		++pSlider;

		*reinterpret_cast<uchar*>(pSlider) = _deviceID;
		++pSlider;

		*reinterpret_cast<uchar*>(pSlider) = static_cast<uchar>(_package.size());
		++pSlider;

		std::memcpy(pSlider, _package.data(), _package.size());
		pSlider += _package.size();

		writeChecksum(pPackage, res.size());

		return res;
	}

	QByteArray Server::unwrap(const QByteArray& _message, uchar* _pDeviceID, bool* _pOK) {
		if (WRAPPER_SIZE > _message.size()) { 
			*_pOK = false;
			return QByteArray();
		}

		const char* pData = _message.data();
		if (HEADER_BYTE != *pData) {
			*_pOK = false;
			return QByteArray();
		}

		quint32 packageSize = static_cast<quint32>(*(pData + PACKAGE_SIZE_POSITION));
		quint32 expectedTotalSize = WRAPPER_SIZE + packageSize;
		if (_message.size() != expectedTotalSize) { 
			*_pOK = false;
			return QByteArray();
		}
		if (!verifyChecksum(pData, expectedTotalSize)) {
			*_pOK = false;
			return QByteArray();
		}

		QByteArray res(packageSize, 0x00);
		std::memcpy(res.data(), pData + PACKAGE_HEAD_POSITION, packageSize);

		*_pDeviceID = *(pData + DEVICE_ID_POSITION);
		*_pOK = true;
		return res;
	}

	void Server::run() {
		if (devices_.empty()) { return; }

		pSerialPort_ = new QSerialPort;
		setupSerialPort();

		bool openRes = false;
		while (true) {
			if (!checkToggle()) { return; }

			pSerialPort_->close();
			openRes = pSerialPort_->open(QIODevice::ReadWrite);

			if (openRes) { break; }
			else { std::this_thread::sleep_for(std::chrono::milliseconds(SERIAL_OPEN_ATTEMPT_PAUSE)); }
		}

		currentDeviceIdx_ = 0;
		while (true) {
			if (!checkToggle()) { return; }

			auto* pDevice = currentDevice();

			QByteArray dataToSend = wrap(pDevice->request(), pDevice->id());
			pSerialPort_->write(dataToSend);
			pSerialPort_->waitForBytesWritten();

			if (!responses_.empty()) { processAllResponses(); }

			bool ok = readResponse(currentDeviceIdx());
			if (!ok) { pDevice->setOnline(false); }

			nextDevice();
		}
	}

	void Server::beforeQuit() {
		pSerialPort_->close();

		delete pSerialPort_;
		pSerialPort_ = nullptr;
	}
}