/*
 * Logger.cpp
 *
 *  Created on: Feb 21, 2019
 *      Author: check
 */
#include "Logger.h"

ClLogger::ClLogger(ClConfiguration *const pConfiguration) :
m_pConfiguration(pConfiguration),
m_nLogLevel(pConfiguration->getValue<int>("SandBox.loglevel")),
m_sLogFile(pConfiguration->getValue<std::string>("SandBox.logfile"))
{
	if (m_pConfiguration == nullptr)
	{
		throw std::runtime_error("pConfiguration must not be 0!");
	}
}

ClLogger* ClLogger::getInstance(ClConfiguration *const pConfiguration)
{
	static ClLogger oLogger{pConfiguration};
	return &oLogger;
}

ClLogger* ClLogger::init(ClConfiguration *pConfig)
{
	return getInstance(pConfig);
}

ClLogger* ClLogger::get()
{
	return getInstance();
}

void ClLogger::debug(const std::string &sMessage)
{
}


