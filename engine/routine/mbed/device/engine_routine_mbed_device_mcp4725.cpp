#include "engine_routine_mbed_device_mcp4725.h"

namespace engine::routine::mbed::device {
	QByteArray MCP4725::request() const {
		if (!pValueEst_) { return QByteArray(); }

		qreal valueEst = 0.;
		pValueEst_->submit(&valueEst);

		if (!std::memcmp(&valueEst, &prevValueEst_, sizeof(qreal))) {
			return QByteArray(1, CHECK_COMMAND);
		}

		quint16 dacCode = 0;

		if (valueEst < 0.) {
			valueEst = 0.;
			dacCode = 0x00'00;
		}
		else if (std::abs(valueEst - 1.) < ZERO_THRESHOLD || 1. < valueEst) {
			valueEst = 1.;
			dacCode = 0x0f'ff;
		}
		else {
			dacCode = static_cast<quint16>(std::round(valueEst * static_cast<qreal>(DAC_AMPLITUDE)) + ZERO_THRESHOLD);
		}
		
		QByteArray res(3, SET_VALUE_COMMAND);
		char* pData = res.data() + 1;
		*reinterpret_cast<quint16*>(pData) = dacCode;

		prevValueEst_ = valueEst;

		return res;
	}

	bool MCP4725::receive(const QByteArray& _package) {
		if (_package.isEmpty()) { return false; }

		const char* pData = _package.data();
		return CHECK_COMMAND == *pData || SET_VALUE_COMMAND == *pData;
	}
}
