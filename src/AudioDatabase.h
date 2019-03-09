/*
 * AudioDatabase.h
 *
 *  Created on: Jan 29, 2019
 *      Author: check
 */

#ifndef AUDIODATABASE_H_
#define AUDIODATABASE_H_

#include "./Logging/Logger.h"
#include <map>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

struct StAudioItem
{
	bool bIsInitialized;
	std::string sSource;
	std::string sAudioInfo;
};

class ClAudioDatabase
{
public:
	ClAudioDatabase();
	~ClAudioDatabase();
	StAudioItem audioItemFromKey(const int nKey);
	bool reinitialize();
private:
	bool initialize();
	StAudioItem audioItemFromFile(const fs::path &oFilePath);
	std::map<int, StAudioItem> m_oAudioMap;
	ClLogger m_oLogger;
};


#endif /* AUDIODATABASE_H_ */
