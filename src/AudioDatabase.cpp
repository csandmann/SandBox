/*
 * AudioDatabase.cpp
 *
 *  Created on: Jan 29, 2019
 *      Author: check
 */
#include "AudioDatabase.h"
#include <fstream>

ClAudioDatabase::ClAudioDatabase() :
m_oLogger(ClLogger("AudioDb"))
{
	initialize();
}

ClAudioDatabase::~ClAudioDatabase() {}

StAudioItem ClAudioDatabase::audioItemFromKey(const int nKey)
{
	StAudioItem stAudioItem;
	if (m_oAudioMap.find(nKey) != m_oAudioMap.end())
	{
		stAudioItem = m_oAudioMap[nKey];
		stAudioItem.bIsInitialized = true;
	}
	else
	{
		stAudioItem.bIsInitialized = false;
	}
	return stAudioItem;
}

bool ClAudioDatabase::reinitialize()
{
	m_oAudioMap.clear();
	return initialize();
}

bool ClAudioDatabase::initialize()
{
	 const fs::path oAudioDbDir("test/AudioDb");
	 if (!fs::exists(oAudioDbDir))
	 {
		 fs::create_directories(oAudioDbDir);
	 }
	 for (const auto &oEntry : fs::directory_iterator(oAudioDbDir))
	 {
		 StAudioItem stAudioItem = audioItemFromFile(oEntry.path());
		 if (!stAudioItem.bIsInitialized)
		 {
			 std::printf("ClAudioDatabase::initialize(): Could not parse %s", oEntry.path().string().c_str());
			 continue;
		 }
		 int nEntry = std::stoi(oEntry.path().filename().string());
		 m_oAudioMap[nEntry] = stAudioItem;
	 }
	 return true;
}

StAudioItem ClAudioDatabase::audioItemFromFile(const fs::path &oPath)
{
	StAudioItem stAudioItem;
	try
	{
		std::string sAudioSource;
		std::string sAudioInfo;
		std::ifstream fin(oPath.string().c_str());
		std::getline(fin, sAudioSource);
		std::getline(fin, sAudioInfo);
		fin.close();
		stAudioItem.sSource = sAudioSource;
		stAudioItem.sAudioInfo = sAudioInfo;
		stAudioItem.bIsInitialized = true;
	}
	catch (std::exception &)
	{
		stAudioItem.bIsInitialized = false;
	}
	return stAudioItem;
}
