/*
 * AudioDatabase.h
 *
 *  Created on: Jan 29, 2019
 *      Author: check
 */

#ifndef AUDIODATABASE_H_
#define AUDIODATABASE_H_

#include <map>
#include "experimental/filesystem"

namespace fs = std::experimental::filesystem;

enum class EAudioSource
{
	SPOTIFY = 0,
	LOCAL = 1
};

struct StAudioItem
{
	bool bKeyFound;
	EAudioSource eSource;
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
};



#endif /* AUDIODATABASE_H_ */
