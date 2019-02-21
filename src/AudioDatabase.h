/*
 * AudioDatabase.h
 *
 *  Created on: Jan 29, 2019
 *      Author: check
 */

#ifndef AUDIODATABASE_H_
#define AUDIODATABASE_H_

#include <map>
#include <thread>
#include "experimental/filesystem"

#include "Configuration.h"

namespace fs = std::experimental::filesystem;

struct StAudioItem
{
	bool bIsInitialized;
	std::string sSource;
	std::string sAudioInfo;
};

class ClAudioDatabase
{
public:
	ClAudioDatabase(const ClConfiguration &oConfig);
	~ClAudioDatabase();
	StAudioItem audioItemFromKey(const int nKey);
	bool reinitialize();
private:
	bool initialize();
	StAudioItem audioItemFromFile(const fs::path &oFilePath);
	std::map<int, StAudioItem> m_oAudioMap;
	const ClConfiguration &m_oConfiguration;
};



#endif /* AUDIODATABASE_H_ */
