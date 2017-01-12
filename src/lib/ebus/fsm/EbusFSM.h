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
#ifndef LIBEBUS_FSM_EBUSFSM_H
#define LIBEBUS_FSM_EBUSFSM_H

#include "IProcess.h"
#include "ILogger.h"
#include "EbusDevice.h"
#include "NQueue.h"
#include "Notify.h"

#include <fstream>
#include <thread>
#include <map>
#include "EbusMessage.h"

using libutils::NQueue;
using std::ofstream;
using std::thread;
using std::map;

namespace libebus
{

class State;

class EbusFSM : public Notify
{
	friend class State;
	friend class OnError;
	friend class Idle;
	friend class Connect;
	friend class Listen;
	friend class LockBus;
	friend class FreeBus;
	friend class Evaluate;
	friend class SendMessage;
	friend class RecvResponse;
	friend class RecvMessage;
	friend class SendResponse;

public:
	EbusFSM(const unsigned char address, const string device, const bool deviceCheck, IProcess* process);

	EbusFSM(const unsigned char address, const string device, const bool deviceCheck, IProcess* process,
		ILogger* logger);

	EbusFSM(const unsigned char address, const string device, const bool deviceCheck, IProcess* process,
		ILogger* logger, const long reopenTime, const long arbitrationTime, const long receiveTimeout,
		const int lockCounter, const int lockRetries, const bool dump, const string dumpFile,
		const long dumpFileMaxSize);

	~EbusFSM();

	void start();
	void stop();

	void open();
	void close();

	bool getDump() const;
	void setDump(bool dump);

	void enqueue(EbusMessage* message);

private:
	thread m_thread;

	bool m_running = true;

	State* m_state = nullptr;
	State* m_forceState = nullptr;

	const unsigned char m_address;

	IProcess* m_process = nullptr;
	ILogger* m_logger = nullptr;

	long m_reopenTime;

	long m_arbitrationTime;
	long m_receiveTimeout;

	int m_lockCounter;
	int m_lockRetries;

	int m_lastResult;

	EbusDevice* m_ebusDevice;

	bool m_dump = false;
	string m_dumpFile;
	long m_dumpFileMaxSize;
	long m_dumpFileSize = 0;
	ofstream m_dumpRawStream;

	NQueue<EbusMessage*> m_ebusMsgQueue;

	void run();

	void changeState(State* state);

	Action active(EbusSequence& eSeq);
	void passive(EbusSequence& eSeq);

	void logError(const string& message);
	void logWarn(const string& message);
	void logInfo(const string& message);
	void logDebug(const string& message);
	void logTrace(const string& message);

};

} // namespace libebus

#endif // LIBEBUS_FSM_EBUSFSM_H
