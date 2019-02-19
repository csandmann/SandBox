/*
 * Configuration.cpp
 *
 *  Created on: Feb 19, 2019
 *      Author: check
 */

#include "Configuration.h"

ClConfiguration::ClConfiguration() :
	m_sConfigFilePath("config.ini")
{
	m_spPropertyTree = std::make_unique<boost::property_tree::ptree>();
	boost::property_tree::ini_parser::read_ini(m_sConfigFilePath.c_str(), *m_spPropertyTree.get());
}



