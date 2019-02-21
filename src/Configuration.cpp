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



