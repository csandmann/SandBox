/*
 * AudioDatabase.cpp
 *
 *  Created on: Jan 29, 2019
 *      Author: check
 */
#include "AudioDatabase.h"
#include <fstream>

ClAudioDatabase::ClAudioDatabase() {}

ClAudioDatabase::~ClAudioDatabase() {}

StAudioItem ClAudioDatabase::audioItemFromKey(int nKey)
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

void ClAudioDatabase::reinitialize()
{
	m_oAudioMap.clear();
	initialize();
}

void ClAudioDatabase::initialize()
{
 for (const auto &oEntry : fs::directory_iterator("./AudioDatabase/"))
 {
	 StAudioItem stAudioItem;
	 audioItemFromFile(oEntry.path(), stAudioItem);
	 int nEntry = std::atoi(oEntry.path().string().c_str());
	 m_oAudioMap[nEntry] = stAudioItem;
 }
}

void ClAudioDatabase::audioItemFromFile(fs::path oPath, StAudioItem &stAudioItem)
{
	std::string sAudioSource;
	std::string sAudioInfo;
	std::ifstream fin(oPath.string().c_str());
	std::getline(fin, sAudioSource);
	std::getline(fin, sAudioInfo);
	fin.close();
	stAudioItem.eSource = static_cast<EAudioSource>(std::atoi(sAudioSource.c_str()));
	stAudioItem.sAudioInfo = sAudioInfo;
}
