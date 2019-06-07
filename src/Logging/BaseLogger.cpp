/*
 * Logger.cpp
 *
 *  Created on: Feb 21, 2019
 *      Author: check
 */
#define _CRT_SECURE_NO_WARNINGS
#include "BaseLogger.h"
#include <sstream>
#include <fstream>
#include <ctime>

ClBaseLogger::ClBaseLogger(const StLoggerConfig *const poLoggerConfig) :
m_oConfig(poLoggerConfig ? *poLoggerConfig : StLoggerConfig())
{
	if (nullptr == poLoggerConfig)
	{
		throw std::runtime_error("poLoggerConfig must not be 0!");
	}
}

ClBaseLogger* ClBaseLogger::getInstance(const StLoggerConfig *const poLoggerConfig)
{
	static ClBaseLogger oLogger{poLoggerConfig};
	return &oLogger;
}

ClBaseLogger* ClBaseLogger::init(const StLoggerConfig oLoggerConfig)
{
	const StLoggerConfig* poLoggerConfig = &oLoggerConfig;
	return getInstance(poLoggerConfig);
}

ClBaseLogger* ClBaseLogger::get()
{
	return getInstance();
}

ELogLevel ClBaseLogger::string2LogLevel(const std::string &sLogLevel)
{
	if (sLogLevel == "ERROR")
		return ELogLevel::ERROR;
	else if (sLogLevel == "WARNING" || sLogLevel == "WARN")
		return ELogLevel::WARNING;
	else if (sLogLevel == "INFO")
		return ELogLevel::INFO;
	else if (sLogLevel == "DEBUG")
		return ELogLevel::DEBUG;
	else
		throw std::runtime_error("Invalid LogLevel: Must be one of ERROR, WARN, INFO, DEBUG");
}


std::string ClBaseLogger::formatLine(const std::string &sMessage, const std::string &sLogLevel, const std::string &sLoggerName /* = "" */)
{
	//format line
	std::string sTimestamp = getTimeStamp();
	std::ostringstream ss;
	if (sLoggerName == "") {
		ss << sTimestamp << " - " << sLogLevel << " - " << sMessage;
	}
	else {
		ss << sTimestamp << " - " << sLogLevel << " - " << sLoggerName << " - " << sMessage;
	}

	return ss.str();
}

void ClBaseLogger::error(const std::string& sMessage, const std::string sLoggerName /* = ""*/)
{
	if (m_oConfig.eLogLevel <= ELogLevel::ERROR)
	{
		std::string sLine = formatLine(sMessage, "ERROR", sLoggerName);
		writeLine(sLine);
	}
}

void ClBaseLogger::warn(const std::string& sMessage, const std::string sLoggerName /* = ""*/)
{
	if (m_oConfig.eLogLevel <= ELogLevel::WARNING)
	{
		std::string sLine = formatLine(sMessage, "WARNING", sLoggerName);
		writeLine(sLine);
	}

}

void ClBaseLogger::info(const std::string& sMessage, const std::string sLoggerName /* = ""*/)
{
	if (m_oConfig.eLogLevel <= ELogLevel::INFO)
	{
		std::string sLine = formatLine(sMessage, "INFO", sLoggerName);
		writeLine(sLine);
	}

}

void ClBaseLogger::debug(const std::string& sMessage, const std::string sLoggerName /* = ""*/)
{
	if (m_oConfig.eLogLevel <= ELogLevel::DEBUG)
	{
		std::string sLine = formatLine(sMessage, "DEBUG", sLoggerName);
		writeLine(sLine);
	}

}

std::vector<char> ClBaseLogger::readFileToVector(const std::string &sFilePath)
{
	std::vector<char> vcOut;
	try
	{
		std::ifstream fsIn;
		fsIn.open(sFilePath.c_str(), std::ios::in);
		fsIn.seekg(0, std::ios::end);
		auto nSize = static_cast<int>(fsIn.tellg());
		fsIn.seekg(0, std::ios::beg);
		//read
		vcOut.resize(nSize+1);
		fsIn.read(vcOut.data(), nSize);
	}
	catch (...) {}
	return vcOut;
}


void ClBaseLogger::writeLine(const std::string &sLine)
{
	//write line
	std::lock_guard<std::mutex> oGuard(m_oWriteMutex);
	if (m_oConfig.sLogFilePath != "")
	{
		//prepend
		FILE* pFile = std::fopen(m_oConfig.sLogFilePath.c_str(), "a");
		std::fprintf(pFile, "%s<br>\n", sLine.c_str());
		std::fclose(pFile);
	}
	else
	{
		std::printf("%s\n", sLine.c_str());
	}
}


std::string ClBaseLogger::getTimeStamp()
{
	//print time to c_str
	std::time_t oCurrentTime = std::time(nullptr);
	char acBuffer[100];
	std::strftime(acBuffer, sizeof(acBuffer), "%d-%m-%Y %H:%M:%S", std::localtime(&oCurrentTime));
	return std::string(acBuffer);
}


