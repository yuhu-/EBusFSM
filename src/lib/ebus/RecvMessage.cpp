/*
 * Copyright (C) Roland Jax 2012-2017 <roland.jax@liwest.at>
 *
 * This file is part of ebuscpp.
 *
 * ebuscpp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ebuscpp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ebuscpp. If not, see http://www.gnu.org/licenses/.
 */

#include "RecvMessage.h"
#include "EvalMessage.h"
#include "Listen.h"

libebus::RecvMessage libebus::RecvMessage::m_recvMessage;

int libebus::RecvMessage::run(EbusFSM* fsm)
{
	int result;
	unsigned char byte;

	// receive Header PBSBNN
	for (int i = 0; i < 3; i++)
	{
		unsigned char byte = 0;

		result = read(fsm, byte, 1, 0);
		if (result != DEV_OK) return (result);

		m_sequence.push_back(byte);
	}

	// check against max. possible size
	if (m_sequence[4] > 16)
	{
		fsm->logWarn(stateMessage(STATE_ERR_NN_WRONG));
		reset(fsm);
		fsm->changeState(Listen::getListen());
		return (DEV_OK);
	}

	// receive Data Dx
	for (int i = 0; i < m_sequence[4]; i++)
	{
		unsigned char byte = 0;

		result = read(fsm, byte, 1, 0);
		if (result != DEV_OK) return (result);

		m_sequence.push_back(byte);
	}

	// 1 for CRC
	int bytes = 1;

	// receive CRC
	for (int i = 0; i < bytes; i++)
	{
		result = read(fsm, byte, 1, 0);
		if (result != DEV_OK) return (result);

		m_sequence.push_back(byte);

		if (byte == SYN || byte == EXT) bytes++;
	}

	fsm->logDebug(m_sequence.toString());

	EbusSequence eSeq;
	eSeq.createMaster(m_sequence);

	if (m_sequence[1] != BROADCAST)
	{
		if (eSeq.getMasterState() == EBUS_OK)
		{
			byte = ACK;
		}
		else
		{
			byte = NAK;
			fsm->logInfo(stateMessage(STATE_WRN_RECV_MSG));
		}

		// send ACK
		result = writeRead(fsm, byte, 0);
		if (result != DEV_OK) return (result);
	}

	if (eSeq.getMasterState() == EBUS_OK)
	{
		if (eSeq.getType() != EBUS_TYPE_MS)
		{
			if (eSeq.getType() == EBUS_TYPE_MM) eSeq.setSlaveACK(byte);

			fsm->logInfo(eSeq.toStringLog());
			fsm->publishEbusSequence(eSeq);
		}

		fsm->changeState(EvalMessage::getEvalMessage());
		return (DEV_OK);
	}

	m_sequence.clear();
	fsm->changeState(Listen::getListen());
	return (DEV_OK);
}

const string libebus::RecvMessage::toString() const
{
	return ("RecvMessage");
}

