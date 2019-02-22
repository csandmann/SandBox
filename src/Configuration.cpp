/*
 * Configuration.cpp
 *
 *  Created on: Feb 19, 2019
 *      Author: check
 */

#include "Configuration.h"
#include "../Resources/generated_config.h"

#include <cstdio>

ClConfiguration::ClConfiguration() :
	m_sConfigFilePath("config.ini")
{
	m_spPropertyTree = std::make_unique<boost::property_tree::ptree>();
	try
	{
		boost::property_tree::ini_parser::read_ini(m_sConfigFilePath.c_str(), *m_spPropertyTree.get());
	}
	catch (...)
	{
		FILE* pConfig = std::fopen(m_sConfigFilePath.c_str(), "w");
		std::fprintf(pConfig, "%s", pcConfig_content);
		std::fclose(pConfig);
		boost::property_tree::ini_parser::read_ini(m_sConfigFilePath.c_str(), *m_spPropertyTree.get());
	}

}

StLoggerConfig* ClConfiguration::getLoggerConfig()
{
	std::string sLogLevel = getValue<std::string>("Logging.logLevel");
	ELogLevel eLogLevel = ClBaseLogger::string2LogLevel(sLogLevel);
	std::string sLogFile = getValue<std::string>("Logging.logLevel");
	m_stLoggerConfig = StLoggerConfig{eLogLevel, sLogFile};
	return &m_stLoggerConfig;
}

StSpotifyConfig* ClConfiguration::getSpotifyConfig()
{
	std::string sClientId = getValue<std::string>("Spotify.clientId");
	std::string sClientSecret = getValue<std::string>("Spotify.clientSecret");
	m_stSpotifyConfig.sClientId = sClientId;
	m_stSpotifyConfig.sClientSecret = sClientSecret;
	return &m_stSpotifyConfig;
}

StReaderConfig* ClConfiguration::getReaderConfig()
{
	return &m_stReaderConfig;
}
