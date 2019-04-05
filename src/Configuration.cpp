/*
 * Configuration.cpp
 *
 *  Created on: Feb 19, 2019
 *      Author: check
 */

#include "Configuration.h"
#include "../Resources/generated_config.h"
#include <chrono>
#include <cstdio>
#include <memory>

ClConfiguration::ClConfiguration() :
	m_sConfigFilePath("config.ini")
{
	initializeConfigFile();
	setupLoggerConfig();
	setupSpotifyConfig();
	setupReaderConfig();
	setupWebserverConfig();
}

void ClConfiguration::initializeConfigFile()
{
	m_spPropertyTree.reset(new boost::property_tree::ptree());
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

void ClConfiguration::setupLoggerConfig()
{
	std::string sLogLevel = getValue<std::string>("Logging.loglevel");
	ELogLevel eLogLevel = ClBaseLogger::string2LogLevel(sLogLevel);
	std::string sLogFilePath = getValue<std::string>("Logging.logfilePath");
	m_oLoggerConfig = StLoggerConfig{eLogLevel, sLogFilePath};
}

void ClConfiguration::setupSpotifyConfig()
{
	std::string sClientId = getValue<std::string>("Spotify.clientId");
	std::string sClientSecret = getValue<std::string>("Spotify.clientSecret");
	std::string sHostname = getValue<std::string>("SandBox.hostname");
	unsigned int nPort = getValue<unsigned int>("SandBox.port");
	std::string sCacheDir = getValue<std::string>("SandBox.cacheDirectory");
	std::string sDevice = getValue<std::string>("Spotify.device");
	std::string sResourceDir = getValue<std::string>("SandBox.resourceDirectory");

	m_oSpotifyConfig.sClientId = sClientId;
	m_oSpotifyConfig.sClientSecret = sClientSecret;
	m_oSpotifyConfig.nPort = nPort;
	m_oSpotifyConfig.sHostname = sHostname;
	m_oSpotifyConfig.oCacheDir = fs::path(sCacheDir);
	m_oSpotifyConfig.sDevice = sDevice;
	m_oSpotifyConfig.sResourceDir = sResourceDir;
}

void ClConfiguration::setupWebserverConfig()
{
	std::string sHostname = getValue<std::string>("SandBox.hostname");
	unsigned int nPort = getValue<unsigned int>("SandBox.port");
	std::string sResourceDir = getValue<std::string>("SandBox.resourceDirectory");
	std::string sLogFilePath = getValue<std::string>("Logging.logfilePath");

	m_oWebserverConfig.nPort = nPort;
	m_oWebserverConfig.sHostname = sHostname;
	m_oWebserverConfig.sResourceDir = sResourceDir;
	m_oWebserverConfig.sLogFilePath = sLogFilePath;
}


void ClConfiguration::setupReaderConfig()
{
	int nReadInterval = getValue<int>("Reader.readIntervalInMs");
	m_oReaderConfig.nReadInterval = std::chrono::milliseconds(nReadInterval);
}

StLoggerConfig ClConfiguration::getLoggerConfig()
{
	return m_oLoggerConfig;
}

StSpotifyConfig ClConfiguration::getSpotifyConfig()
{
	return m_oSpotifyConfig;
}

StWebserverConfig ClConfiguration::getWebserverConfig()
{
	return m_oWebserverConfig;
}


StReaderConfig ClConfiguration::getReaderConfig()
{
	return m_oReaderConfig;
}
