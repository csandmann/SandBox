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

class ClConfiguration
{
public:
	ClConfiguration();
	template<typename T> T getValue(const char* sProperty) const;
private:
	std::string m_sConfigFilePath;
	std::unique_ptr<boost::property_tree::ptree> m_spPropertyTree;
};


template<typename T> T ClConfiguration::getValue(const char* pcProperty) const
{
	return m_spPropertyTree->get<T>(pcProperty);
}

#endif /* SRC_CONFIGURATION_H_ */
