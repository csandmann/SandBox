/*
 * Logger.cpp
 *
 *  Created on: Feb 22, 2019
 *      Author: check
 */


#include "Logger.h"
#include <ctime>
#include <sstream>

ClLogger::ClLogger(const std::string &sLoggerName) :
m_sLoggerName(sLoggerName)
{}

void ClLogger::info(const std::string& sMessage)
{
	ClBaseLogger::get()->info(sMessage, m_sLoggerName);
}

void ClLogger::warn(const std::string& sMessage)
{
	ClBaseLogger::get()->warn(sMessage, m_sLoggerName);
}

void ClLogger::error(const std::string& sMessage)
{
	ClBaseLogger::get()->error(sMessage, m_sLoggerName);
}

void ClLogger::debug(const std::string& sMessage)
{
	ClBaseLogger::get()->debug(sMessage, m_sLoggerName);
}


