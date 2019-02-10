/*
 * Manager.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: check
 */

#include "Manager.h"
#include <chrono>

ClManager::ClManager(const std::shared_ptr<ClPlayer> &spPlayer, const std::shared_ptr<ClReaderBase> &spReader, const std::shared_ptr<ClAudioDatabase> &spAudioDb):
m_spPlayer(spPlayer),
m_spReader(spReader),
m_spAudioDb(spAudioDb),
m_bInterruptRequested(false),
m_nWaitTime(10)
{}

ClManager::~ClManager() {}

void ClManager::start()
{
	while (!m_bInterruptRequested)
	{
		StReaderMessage stMsg = m_spReader->getMessage();
		if (playbackNeeded(stMsg))
		{
			StAudioItem stAudioItem = m_spAudioDb->audioItemFromKey(stMsg.nKey);
			if (stAudioItem.bKeyFound)
			{
				m_spPlayer->play(stAudioItem);
			}
			m_stCurrentMsg = stMsg;
		}
		else if (playbackToStop(stMsg))
		{
			m_spPlayer->stop();
			m_stCurrentMsg = stMsg;
		}
		//sleep
		std::this_thread::sleep_for(m_nWaitTime);
	}

}

void ClManager::stop()
{
	m_bInterruptRequested = true;
	m_spReader->stop();
	m_spPlayer->stop();
}

bool ClManager::playbackNeeded(const StReaderMessage& stMsg)
{
	return (stMsg.eStatus == EReaderStatus::DETECTED && stMsg.nKey != m_stCurrentMsg.nKey);
}

bool ClManager::playbackToStop(const StReaderMessage& stMsg)
{
	return (stMsg.eStatus == EReaderStatus::EMPTY && m_stCurrentMsg.eStatus == EReaderStatus::DETECTED);
}
