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

#include "Option.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

using std::cerr;
using std::endl;
using std::hex;
using std::dec;

Option& Option::getInstance(const char* command, const char* argument)
{
	static Option instance(command, argument);
	return (instance);
}

Option::~Option()
{
	m_opts.clear();
	m_optvals.clear();
}

void Option::setVersion(const char* version)
{
	m_version = version;
}

void Option::addText(const char* text)
{
	opt_t opt;
	opt.name = "__text_only__";
	opt.shortname = "";
	opt.datatype = dt_none;
	opt.optiontype = ot_none;
	opt.description = text;
	m_opts.push_back(opt);
}

void Option::addOption(const char* name, const char* shortname, OptVal optval,
	DataType datatype, OptionType optiontype, const char* description)
{
	if (strlen(name) != 0)
	{
		m_optvals[name] = optval;

		opt_t opt;
		opt.name = name;
		opt.shortname = shortname;
		opt.datatype = datatype;
		opt.optiontype = optiontype;
		opt.description = description;
		m_opts.push_back(opt);
	}
}

bool Option::parseArgs(int argc, char* argv[])
{
	vector<string> _argv(argv, argv + argc);
	m_argv = _argv;
	int i;
	bool lastOption = false;

	// walk through all arguments
	for (i = 1; i < argc; i++)
	{

		// find option with long format '--'
		if (_argv[i].rfind("--") == 0 && _argv[i].size() > 2)
		{

			// is next item an added argument?
			if (i + 1 < argc
				&& _argv[i + 1].rfind("-", 0) == string::npos)
			{
				if (checkOption(_argv[i].substr(2),
					_argv[i + 1]) == false) return (false);
			}
			else
			{
				if (checkOption(_argv[i].substr(2), "")
					== false) return (false);
			}

			lastOption = true;

			// find option with short format '-'
		}
		else if (_argv[i].rfind("-") == 0 && _argv[i].size() > 1)
		{

			// walk through all characters
			for (size_t j = 1; j < _argv[i].size(); j++)
			{

				// only last charater could have an argument
				if (i + 1 < argc
					&& _argv[i + 1].rfind("-", 0)
						== string::npos
					&& j + 1 == _argv[i].size())
				{
					if (checkOption(_argv[i].substr(j, 1),
						_argv[i + 1]) == false)
						return (false);
				}
				else
				{
					if (checkOption(_argv[i].substr(j, 1),
						"") == false) return (false);
				}
			}

			lastOption = true;

		}
		else
		{
			// break loop with command
			if (lastOption == false && strlen(m_withCommand) != 0)
			{
				break;
			}
			else
			{
				lastOption = false;
			}

		}

	}

	if (i < argc && strlen(m_withCommand) != 0)
	{
		// save command
		m_command = _argv[i];

		if (strlen(m_withArgument) != 0)
		{
			// save args of command
			for (++i; i < argc; i++)
				m_arguments.push_back(_argv[i]);

		}
	}

	return (true);
}

int Option::numArgs() const
{
	return (m_arguments.size());
}

string Option::getArg(const int num) const
{
	return (m_arguments[num]);
}

string Option::getCommand() const
{
	return (m_command);
}

bool Option::missingCommand() const
{
	return (m_command.size() == 0 ? true : false);
}

Option::Option(const char* command, const char* argument)
	: m_version(NULL), m_withCommand(command), m_withArgument(argument)
{
}

bool Option::checkOption(const string& option, const string& value)
{
	if (strcmp(option.c_str(), "options") == 0) return (toStringOptions());

	if (strcmp(option.c_str(), "version") == 0) return (toStringVersion());

	if (strcmp(option.c_str(), "h") == 0
		|| strcmp(option.c_str(), "help") == 0) return (toStringHelp());

	for (o_it = m_opts.begin(); o_it < m_opts.end(); ++o_it)
	{
		if (o_it->shortname == option || o_it->name == option)
		{

			// need this option and argument?
			if (o_it->optiontype == ot_mandatory
				&& value.size() == 0)
			{
				cerr << endl << "option requires an argument '"
					<< option << "'" << endl;
				return (toStringHelp());
			}

			// add given value to option
			if ((o_it->optiontype == ot_optional
				&& value.size() != 0)
				|| o_it->optiontype != ot_optional)
				setOptVal(o_it->name, value, o_it->datatype);

			return (true);
		}
	}

	cerr << endl << "unknown option '" << option << "'" << endl;
	return (toStringHelp());
}

void Option::setOptVal(const char* option, const string value,
	DataType datatype)
{
	switch (datatype)
	{
	case dt_bool:
		m_optvals[option] = true;
		break;
	case dt_hex:
		m_optvals[option] = strtol(value.c_str(), NULL, 16);
		break;
	case dt_int:
		m_optvals[option] = strtol(value.c_str(), NULL, 10);
		break;
	case dt_long:
		m_optvals[option] = strtol(value.c_str(), NULL, 10);
		break;
	case dt_float:
		m_optvals[option] = static_cast<float>(strtod(value.c_str(),
		NULL));
		break;
	case dt_string:
		m_optvals[option] = value.c_str();
		break;
	default:
		break;
	}
}

bool Option::toStringVersion() const
{
	cerr << m_version << endl;

	return (false);
}

bool Option::toStringHelp()
{
	cerr << endl << "Usage:" << endl << "  "
		<< m_argv[0].substr(m_argv[0].find_last_of("/\\") + 1)
		<< " [Options...]";

	if (strlen(m_withCommand) != 0)
	{
		if (strlen(m_withArgument) != 0)
		{
			cerr << " " << m_withCommand << " " << m_withArgument
				<< endl << endl;
		}
		else
		{
			cerr << " " << m_withCommand << endl << endl;
		}
	}
	else
	{
		cerr << endl << endl;
	}

	for (o_it = m_opts.begin(); o_it < m_opts.end(); ++o_it)
	{
		if (strcmp(o_it->name, "__text_only__") == 0)
		{
			cerr << o_it->description << endl;
		}
		else
		{
			const char* c =
				(strlen(o_it->shortname) == 1) ?
					o_it->shortname : " ";
			cerr << ((strcmp(c, " ") == 0) ? " " : "-") << c
				<< " | --" << o_it->name << "\t"
				<< o_it->description << endl;
		}
	}

	cerr << endl << "   | --options\n   | --version\n-h | --help" << endl
		<< endl;

	return (false);
}

bool Option::toStringOptions()
{
	cerr << endl << "Options:" << endl << endl;

	for (o_it = m_opts.begin(); o_it < m_opts.end(); ++o_it)
	{
		if (strcmp(o_it->name, "__text_only__") == 0) continue;

		const char* c =
			(strlen(o_it->shortname) == 1) ? o_it->shortname : " ";
		cerr << ((strcmp(c, " ") == 0) ? " " : "-") << c << " | --"
			<< o_it->name << " = ";
		if (o_it->datatype == dt_bool)
		{
			if (getOptVal<bool>(o_it->name) == true)
			{
				cerr << "yes" << endl;
			}
			else
			{
				cerr << "no" << endl;
			}
		}
		else if (o_it->datatype == dt_hex)
		{
			cerr << hex << getOptVal<int>(o_it->name) << dec
				<< endl;
		}
		else if (o_it->datatype == dt_int)
		{
			cerr << getOptVal<int>(o_it->name) << endl;
		}
		else if (o_it->datatype == dt_long)
		{
			cerr << getOptVal<long>(o_it->name) << endl;
		}
		else if (o_it->datatype == dt_float)
		{
			cerr << getOptVal<float>(o_it->name) << endl;
		}
		else if (o_it->datatype == dt_string)
		{
			cerr << getOptVal<const char*>(o_it->name) << endl;
		}

	}

	cerr << endl;

	return (false);
}
