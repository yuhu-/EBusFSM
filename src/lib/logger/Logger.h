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

#ifndef LIBLOGGER_LOGGER_H
#define LIBLOGGER_LOGGER_H

#include "LogHandler.h"

class Logger
{

public:
	explicit Logger(const char* function);

	void start();
	void stop();

	void setLevel(const Level& level);
	void setLevel(const string& level);

	void addConsole();
	void addFile(const string& file);

	void log(const Level& level, const string data, ...);

private:
	LogHandler& m_logHandler;
	const char* m_function;

};

#endif // LIBLOGGER_LOGGER_H
