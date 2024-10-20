#include "engine_routine_udp_util.h"

namespace engine::routine::udp::util {
	char xorChecksum(const char* _pSrc, quint32 _size) {
		char result = 0x00;
		for (quint32 i = 0; i + 1 < _size; ++i) {
			result ^= *_pSrc;
			++_pSrc;
		}
		return result;
	}

	bool verifyXorChecksum(const char* _pSrc, quint32 _size) {
		return *(_pSrc + _size - 1) == xorChecksum(_pSrc, _size);
	}

	void writeXorChecksum(char* _pDst, quint32 _size) {
		*(_pDst + _size - 1) = xorChecksum(_pDst, _size);
	}

	bool parseMessage(const QByteArray& _src, char& _rDstRWFlag, QString& _rDstId, QByteArray& _rDstValue, bool& _rDstIsManual) {
		if (MARKERS_SIZE > _src.size()) { return false; }

		// verify header
		const char* pData = _src.data();
		if (HEADER != *reinterpret_cast<const quint16*>(pData)) { return false; }
		pData += sizeof(quint16);

		// verify package size
		auto packageSize = static_cast<quint32>(*reinterpret_cast<const quint16*>(pData));
		if (WRAPPER_SIZE + packageSize != _src.size()) { return false; }

		// verify checksum
		if (!verifyXorChecksum(_src.data(), _src.size())) { return false; }
		pData += sizeof(quint16);

		// read RW flag
		char rwFlag = *pData;
		switch (rwFlag) {
		case READ_FLAG:
		case WRITE_FLAG: {
			break;
		}
		default:
			return false;
		}
		++pData;

		// verify id size
		auto idSize = static_cast<quint32>(*reinterpret_cast<const quint16*>(pData));
		if (packageSize < 2 * sizeof(char) + 2 * sizeof(quint16) + idSize) { return false; }
		pData += sizeof(quint16);

		// get id data
		QByteArray idBytes(idSize, 0x00);
		std::memcpy(idBytes.data(), pData, idSize);
		pData += idSize;

		// get value size
		auto valueSize = static_cast<quint32>(*reinterpret_cast<const quint16*>(pData));
		if (packageSize < 2 * sizeof(char) + 2 * sizeof(quint16) + idSize + valueSize) { return false; }
		pData += sizeof(quint16);

		// get value data
		QByteArray value(valueSize, 0x00);
		std::memcpy(value.data(), pData, valueSize);
		pData += valueSize;

		// get malual control flag
		bool isManual = static_cast<bool>(*pData);

		_rDstRWFlag = rwFlag;
		_rDstId = QString::fromUtf8(idBytes);
		_rDstValue = value;
		_rDstIsManual = isManual;

		return true;
	}
}
