/*
 * Manager.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: check
 */

#include "Manager.h"
#include <chrono>

ClManager::ClManager(ClReaderBase *const poReader, ClAudioDatabase *const poAudioDb) :
m_poReader(poReader),
m_poAudioDb(poAudioDb),
m_poActivePlayer(nullptr),
m_bInterruptRequested(false),
m_nWaitTime(500)
{}

ClManager::~ClManager(){}

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
					if (poPlayer->getIdentifier() == stAudioItem.sSource)
					{
						poPlayer->play(stAudioItem.sAudioInfo.c_str());
						m_poActivePlayer = poPlayer;
					}
				}

			}
			m_stCurrentMsg = stMsg;
		}
		else if (playbackToStop(stMsg))
		{
			if (m_poActivePlayer != nullptr){
				m_poActivePlayer->stop();
				m_poActivePlayer = nullptr;
			}
			m_stCurrentMsg = StReaderMessage{stMsg.eStatus, -1};
		}
		//sleep
		std::this_thread::sleep_for(m_nWaitTime);
	}

}

void ClManager::stop()
{
	m_bInterruptRequested = true;
	m_poReader->stop();
	if (m_poActivePlayer != nullptr) {
		m_poActivePlayer->stop();
	}
}

bool ClManager::playbackNeeded(const StReaderMessage& stMsg)
{
	return (stMsg.eStatus == EReaderStatus::DETECTED && stMsg.nKey != m_stCurrentMsg.nKey);
}

void ClManager::registerPlayer(ClPlayerBase *const poPlayer)
{
	m_vpPlayers.push_back(poPlayer);
}

bool ClManager::playbackToStop(const StReaderMessage& stMsg)
{
	return (stMsg.eStatus == EReaderStatus::EMPTY && m_stCurrentMsg.eStatus == EReaderStatus::DETECTED);
}
