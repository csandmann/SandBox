/*
 * Logger.h
 *
 *  Created on: Feb 21, 2019
 *      Author: check
 */

#ifndef SRC_LOGGER_H_
#define SRC_LOGGER_H_

#include "Configuration.h"
#include <string>

enum class ELogLevel
{
	ERROR = 0,
	WARNING = 1,
	INFO = 2
};

class ClLogger
{
public:
	ClLogger(ClConfiguration* const pConfiguration);
	static ClLogger* init(ClConfiguration *pConfig);
	static ClLogger* get();
	void debug(const std::string &sMessage);

private:
	ClLogger(ClLogger const& ) = delete;
	void operator=(ClLogger const&) = delete;
	static ClLogger* getInstance(ClConfiguration *const pConfiguration = nullptr);
	void writeLine(const char* pcPrefix, const char* pcMessage) const;

	const ClConfiguration *m_pConfiguration;
	const int m_nLogLevel;
	const std::string m_sLogFile;
};




#endif /* SRC_LOGGER_H_ */
