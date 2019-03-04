/*
 * Configuration.h
 *
 *  Created on: Feb 19, 2019
 *      Author: check
 */

#ifndef SRC_CONFIGURATION_H_
#define SRC_CONFIGURATION_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "Logging/BaseLogger.h"
#include "PlayerPlugins/PlayerSpotify/PlayerSpotify.h"
#include "ReaderPlugins/ReaderFile.h"
#include "Webserver.h"

class ClConfiguration
{
public:
	ClConfiguration();
	StLoggerConfig getLoggerConfig();
	StSpotifyConfig getSpotifyConfig();
	StReaderConfig getReaderConfig();
	StWebserverConfig getWebserverConfig();
private:
	template<typename T> T getValue(const char* sProperty) const;
	std::string m_sConfigFilePath;
	std::unique_ptr<boost::property_tree::ptree> m_spPropertyTree;

	void initializeLogFile();
	void setupLoggerConfig();
	void setupSpotifyConfig();
	void setupReaderConfig();
	void setupWebserverConfig();

	StLoggerConfig m_oLoggerConfig;
	StSpotifyConfig m_oSpotifyConfig;
	StReaderConfig m_oReaderConfig;
	StWebserverConfig m_oWebserverConfig;
};


template<typename T> T ClConfiguration::getValue(const char* pcProperty) const
{
	return m_spPropertyTree->get<T>(pcProperty);
}

#endif /* SRC_CONFIGURATION_H_ */
