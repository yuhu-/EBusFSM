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

#include "Network.h"

using std::make_unique;

Network::Network(const bool local, const int port)
{
	m_tcpAcceptor = make_unique<TCPAcceptor>(local, port, &m_netMsgQueue);
	m_tcpAcceptor->start();

	m_udpReceiver = make_unique<UDPReceiver>(local, port, &m_netMsgQueue);
	m_udpReceiver->start();
}

Network::~Network()
{
	if (m_udpReceiver != nullptr) m_udpReceiver->stop();

	if (m_tcpAcceptor != nullptr) m_tcpAcceptor->stop();

	while (m_netMsgQueue.size() > 0)
		delete m_netMsgQueue.dequeue();
}

NetMessage* Network::dequeue()
{
	return (m_netMsgQueue.dequeue());
}

