/*
 * Manager.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: check
 */

#include "Manager.h"
#include <chrono>

ClManager::ClManager(ClReaderBase *poReader) :
m_poReader(poReader),
m_poActivePlayer(nullptr),
m_bInterruptRequested(false),
m_nWaitTime(500)
{}

ClManager::~ClManager(){}

void ClManager::start()
{
	while (!m_bInterruptRequested)
	{
		//check if there are messages to write
		for (auto *poPlayer : m_vpPlayers)
		{
			std::vector<unsigned char> vcData = poPlayer->getMessageToWrite();
			if (vcData.size() > 0)
			{
				ReaderMessage::StCardData stCardData;
				stCardData.nVersionNumber = '0';
				stCardData.sPlayerIdentifier = poPlayer->getIdentifier();
				stCardData.vcPlayerMessage = vcData;
				m_poReader->requestWrite(stCardData);
			}
		}
        //read message and find a player to play it
		ReaderMessage::StMessage stMsg = m_poReader->getMessage();
		if (playbackNeeded(stMsg))
		{			
            for (auto poPlayer : m_vpPlayers)
            {
                if (poPlayer->getIdentifier() == stMsg.stCardData.sPlayerIdentifier)
                {
                    poPlayer->execute(stMsg.stCardData.vcPlayerMessage);
                    m_poActivePlayer = poPlayer;
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
			ReaderMessage::resetMessage(m_stCurrentMsg);
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

bool ClManager::playbackNeeded(const ReaderMessage::StMessage& stMsg)
{
	bool b1 = stMsg.eStatus == ReaderMessage::EStatus::DETECTED;
	bool b2 = stMsg.stCardData != m_stCurrentMsg.stCardData;
	return (b1 && b2);
}

void ClManager::registerPlayer(ClPlayerBase *const poPlayer)
{
	m_vpPlayers.push_back(poPlayer);
}

bool ClManager::playbackToStop(const ReaderMessage::StMessage& stMsg)
{
	return (stMsg.eStatus == ReaderMessage::EStatus::EMPTY && m_stCurrentMsg.eStatus == ReaderMessage::EStatus::DETECTED);
}
