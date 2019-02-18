/*
 * Manager.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: check
 */

#include "Manager.h"
#include <chrono>

ClManager::ClManagerClManager(const ClReaderBase *poReader, const ClAudioDatabase *poAudioDb) :
m_poReader(poReader),
m_poAudioDb(poAudioDb),
m_bInterruptRequested(false),
m_nWaitTime(500)
{}

ClManager::~ClManager() {}

void ClManager::start()
{
	while (!m_bInterruptRequested)
	{
		StReaderMessage stMsg = m_poReader->getMessage();
		if (playbackNeeded(stMsg))
		{
			StAudioItem stAudioItem = m_poAudioDb->audioItemFromKey(stMsg.nKey);
			if (stAudioItem.bIsInitialized)
			{
				for (auto poPlayer : m_vpPlayers)
				{
					if (poPlayer->getIdentifier() == stAudioItem.eSource)
					{
						poPlayer->play(stAudioItem.sAudioInfo.c_str());
					}
				}

			}
			m_stCurrentMsg = stMsg;
		}
		else if (playbackToStop(stMsg))
		{
			m_poPlayer->stop();
			m_stCurrentMsg = stMsg;
		}
		//sleep
		std::this_thread::sleep_for(m_nWaitTime);
	}

}

void ClManager::stop()
{
	m_bInterruptRequested = true;
	m_poReader->stop();
	m_poPlayer->stop();
}

bool ClManager::playbackNeeded(const StReaderMessage& stMsg)
{
	return (stMsg.eStatus == EReaderStatus::DETECTED && stMsg.nKey != m_stCurrentMsg.nKey);
}

bool ClManager::playbackToStop(const StReaderMessage& stMsg)
{
	return (stMsg.eStatus == EReaderStatus::EMPTY && m_stCurrentMsg.eStatus == EReaderStatus::DETECTED);
}
