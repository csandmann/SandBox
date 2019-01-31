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
	EAudioSource eSource;
	std::string sAudioInfo;
};

class ClAudioDatabase
{
public:
	ClAudioDatabase();
	~ClAudioDatabase();
	bool isKeyRegistered(int nKey);
	bool audioEntryFromKey(int nKey, StAudioItem &stAudioItem);
	void reinitialize();
private:
	void initialize();
	void audioItemFromFile(fs::path oFilePath, StAudioItem &stAudioItem);
	std::map<int, StAudioItem> m_oAudioMap;
};



#endif /* AUDIODATABASE_H_ */
