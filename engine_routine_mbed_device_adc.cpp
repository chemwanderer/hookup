#include "engine_routine_mbed_device_adc.h"

namespace engine::routine::mbed::device {
	QByteArray ADC::request() const {
		return QByteArray(sizeof(char), READ_ALL_COMMAND);
	}

	bool ADC::receive(const QByteArray& _package) {
		if (READ_ALL_RESPONSE_PACKAGE_SIZE != _package.size()) { return false; }

		const char* pData = _package.data();
		if (READ_ALL_COMMAND != *pData) { return false; }
		++pData;

		for (auto* pPrm : channels_) {
			if (pPrm) {
				qreal value = static_cast<qreal>(*reinterpret_cast<const quint16*>(pData)) * INV_MAX_USHORT;
				pPrm->accept(&value);
			}
			pData += sizeof(quint16);
		}

		return true;
	}
}