/*
 * AudioDatabase.cpp
 *
 *  Created on: Jan 29, 2019
 *      Author: check
 */
#include "AudioDatabase.h"
#include <fstream>

ClAudioDatabase::ClAudioDatabase() {
	initialize();
}

ClAudioDatabase::~ClAudioDatabase() {}

StAudioItem ClAudioDatabase::audioItemFromKey(const int nKey)
{
	StAudioItem stAudioItem;
	if (m_oAudioMap.find(nKey) != m_oAudioMap.end())
	{
		stAudioItem = m_oAudioMap[nKey];
		stAudioItem.bKeyFound = true;
	}
	else
	{
		stAudioItem.bKeyFound = false;
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
 try
 {
	 for (const auto &oEntry : fs::directory_iterator("/home/check/test/AudioDatabase/"))
	 {
		 StAudioItem stAudioItem = audioItemFromFile(oEntry.path());
		 int nEntry = std::atoi(oEntry.path().filename().string().c_str());
		 m_oAudioMap[nEntry] = stAudioItem;
	 }
	 return true;
 }
 catch (std::exception &e)
 {
	 std::printf("bool ClAudioDatabase::initialize(): %s", e.what());
	 return false;
 }
}

StAudioItem ClAudioDatabase::audioItemFromFile(const fs::path &oPath)
{
	std::string sAudioSource;
	std::string sAudioInfo;
	std::ifstream fin(oPath.string().c_str());
	std::getline(fin, sAudioSource);
	std::getline(fin, sAudioInfo);
	fin.close();
	StAudioItem stAudioItem;
	stAudioItem.eSource = static_cast<EAudioSource>(std::atoi(sAudioSource.c_str()));
	stAudioItem.sAudioInfo = sAudioInfo;
	return stAudioItem;
}
