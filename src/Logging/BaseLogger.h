/*
 * Logger.h
 *
 *  Created on: Feb 21, 2019
 *      Author: check
 */

#ifndef SRC_LOGFILEWRITER_H_
#define SRC_LOGFILEWRITER_H_

#include <string>
#include <mutex>

enum class ELogLevel
{
	ERROR = 4,
	WARNING = 3,
	INFO = 2,
	DEBUG = 1
};


struct StLoggerConfig
{
	ELogLevel eLogLevel;
	std::string sLogFilePath;
};


class ClBaseLogger
{
public:
	ClBaseLogger(const StLoggerConfig *const poLoggerConfig);
	static ClBaseLogger* init(const StLoggerConfig *const poLoggerConfig);
	static ClBaseLogger* get();
	static ELogLevel string2LogLevel(const std::string &sLogLevel);

	void error(const std::string &sMessage, const std::string sLoggerName="");
	void warn(const std::string &sMessage, const std::string sLoggerName="");
	void info(const std::string &sMessage, const std::string sLoggerName="");
	void debug(const std::string &sMessage, const std::string sLoggerName="");

private:
	ClBaseLogger(ClBaseLogger const& ) = delete;
	void operator=(ClBaseLogger const&) = delete;
	static ClBaseLogger* getInstance(const StLoggerConfig *const poLoggerConfig = nullptr);
	std::string getTimeStamp();
	std::string formatLine(const std::string &sMessage, const std::string &sLogLevel, const std::string &sLoggerName = "");
	void writeLine(const std::string &sLine);

	const StLoggerConfig *m_poConfig;
	std::mutex m_oWriteMutex;
};




#endif /* SRC_LOGFILEWRITER_H_ */
