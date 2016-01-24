/*
 * Copyright (C) Roland Jax 2012-2016 <roland.jax@liwest.at>
 *
 * This file is part of ebusgate.
 *
 * ebusgate is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ebusgate is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ebusgate. If not, see http://www.gnu.org/licenses/.
 */

#ifndef LIBEBUS_EBUSSEQUENCE_H
#define LIBEBUS_EBUSSEQUENCE_H

#include "Sequence.h"

#define EBUS_OK          0 // success
#define EBUS_ERR_SHORT  -1 // sequence to short
#define EBUS_ERR_LONG   -2 // sequence to long
#define EBUS_ERR_BYTES  -3 // to much data bytes
#define EBUS_ERR_CRC    -4 // crc differs
#define EBUS_ERR_ACK    -5 // ack byte wrong

#define EBUS_TYPE_BC     0
#define EBUS_TYPE_MM     1
#define EBUS_TYPE_MS     2

class EbusSequence
{

public:
	EbusSequence();
	explicit EbusSequence(Sequence& seq);

	void decodeSequence(Sequence& seq);

	void createMaster(const string& str);
	void createMaster(Sequence& seq);

	void createSlave(const string& str);
	void createSlave(Sequence& seq);

	void clear();

	Sequence getMaster() const;
	unsigned char getMasterCRC() const;
	int getMasterState() const;

	Sequence getSlave() const;
	unsigned char getSlaveCRC() const;
	int getSlaveState() const;

	void setType(const unsigned char& byte);
	int getType() const;

	bool isValid() const;

	const string toStringFull();

	const string toStringMaster();
	const string toStringMasterCRC();
	const string toStringMasterACK();

	const string toStringSlave();
	const string toStringSlaveCRC();
	const string toStringSlaveACK();

private:
	int m_type = -1;

	Sequence m_master;
	unsigned char m_masterCRC = 0;
	unsigned char m_masterACK = 0;
	int m_masterState = EBUS_OK;

	Sequence m_slave;
	unsigned char m_slaveCRC = 0;
	unsigned char m_slaveACK = 0;
	int m_slaveState = EBUS_OK;

	static const string errorText(const int error);

};

#endif // LIBEBUS_EBUSSEQUENCE_H

