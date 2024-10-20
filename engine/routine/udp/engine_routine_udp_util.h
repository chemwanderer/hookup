#pragma once
////////////////////////////////////////////////////////////
//                                                        //
// engine_routine_udp_util.h containes a set of auxiliary //
// functions for data exchange between client and server  //
// devices. Data exchange is carried out within the       //
// framework of master-slave paradigm, where the "client" //
// system performes the control by sending initializing   //
// requests for parameters read & write operations.       //
//                                                        //
// General message structure (both request & response):   //
// 0     | 0xaa                                           //
// 1     | 0x55                                           //
// 2     | package size N (lo)				  //
// 3     | package size N (hi)                            //
// 4     | package                                        //
// ...   | package                                        //
// 3 + N | package                                        //
// 4 + N | checksum                                       //
//							  //
// Internal package structure:                            //
// 0         | read/write code                            //
// 1         | string id size M (lo)                      //
// 2         | string id size M (hi)                      //
// 3         | string id                                  //
// ...       | string id				  //
// 2 + M     | string id                                  //
// 3 + M     | value size K (lo)                          //
// 4 + M     | value size K (hi)                          //
// 5 + M     | value                                      //
// ...       | value                                      //
// 4 + M + K | value                                      //
// 5 + M + K | manual control flag                        //
//                                                        //
////////////////////////////////////////////////////////////
#include <QByteArray>
#include <QString>
#include "engine/engine_i_value.h"

namespace engine::routine::udp::util {
	static constexpr char READ_FLAG = 0x01;
	static constexpr char WRITE_FLAG = 0x02;

	static constexpr quint16 HEADER = 0x55aa;
	static constexpr quint32 WRAPPER_SIZE = 5;
	static constexpr quint32 MARKERS_SIZE = WRAPPER_SIZE + 2 * sizeof(char) + 2 * sizeof(quint16);

	char xorChecksum(const char*, quint32);
	bool verifyXorChecksum(const char*, quint32);
	void writeXorChecksum(char*, quint32);
	
	template<char _rwFlag>
	QByteArray message(const QString& _id, const QByteArray& _value, bool _isManual);

	bool parseMessage(const QByteArray& _src, 
		char& _rDstRWFlag, QString& _rDstId, QByteArray& _rDstValue, bool& _rDstIsManual);
}

template<char _rwFlag>
QByteArray engine::routine::udp::util::message(const QString& _id, const QByteArray& _value, bool _isManual) {
	QByteArray idBytes = _id.toUtf8();

	// create an array of bytes
	QByteArray result(MARKERS_SIZE + idBytes.size() + _value.size(), 0x00);
	char* pData = result.data();

	// write header
	*reinterpret_cast<quint16*>(pData) = HEADER;
	pData += sizeof(quint16);

	// write package size
	quint16 packageSize = static_cast<quint16>(idBytes.size() + _value.size() + 2 * sizeof(char) + 2 * sizeof(quint16));
	*reinterpret_cast<quint16*>(pData) = packageSize;
	pData += sizeof(quint16);

	// write R/W flag
	*pData = _rwFlag;
	++pData;

	// write string id size
	*reinterpret_cast<quint16*>(pData) = static_cast<quint16>(idBytes.size());
	pData += sizeof(quint16);

	// write string id data
	std::memcpy(pData, idBytes.data(), idBytes.size());
	pData += idBytes.size();

	// write value size
	*reinterpret_cast<quint16*>(pData) = static_cast<quint16>(_value.size());
	pData += sizeof(quint16);

	// write value data
	if (!_value.isEmpty()) {
		std::memcpy(pData, _value.data(), _value.size());
		pData += _value.size();
	}

	// write manual control flag
	*pData = static_cast<char>(_isManual);

	writeXorChecksum(result.data(), result.size());

	return result;
}
