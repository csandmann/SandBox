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

#include "./Logging/BaseLogger.h"
#include "./PlayerPlugins/PlayerSpotify.h"
#include "./ReaderPlugins/ReaderFile.h"

class ClConfiguration
{
public:
	ClConfiguration();
	StLoggerConfig* getLoggerConfig();
	StSpotifyConfig* getSpotifyConfig();
	StReaderConfig* getReaderConfig();
private:
	template<typename T> T getValue(const char* sProperty) const;
	std::string m_sConfigFilePath;
	std::unique_ptr<boost::property_tree::ptree> m_spPropertyTree;

	StLoggerConfig m_stLoggerConfig;
	StSpotifyConfig m_stSpotifyConfig;
	StReaderConfig m_stReaderConfig;
};


template<typename T> T ClConfiguration::getValue(const char* pcProperty) const
{
	return m_spPropertyTree->get<T>(pcProperty);
}

#endif /* SRC_CONFIGURATION_H_ */
