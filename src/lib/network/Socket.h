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

#ifndef LIBNETWORK_SOCKET_H
#define LIBNETWORK_SOCKET_H

#include <string>

#include <netinet/in.h>

using std::string;

namespace libnetwork
{

class Socket
{
	friend class Client;
	friend class Server;

public:
	~Socket();

	ssize_t send(const char* buffer, const size_t len, const struct sockaddr_in* address, const socklen_t addrlen);
	ssize_t recv(char* buffer, const size_t len, struct sockaddr_in* address, socklen_t* addrlen);

	string getIP() const;

	long getPort() const;

	int getFD() const;

	bool isValid();

private:
	int m_sfd;

	string m_ip;

	long m_port;

	Socket(const int sfd, const struct sockaddr_in* address);

};

} // namespace libnetwork

#endif // LIBNETWORK_SOCKET_H

