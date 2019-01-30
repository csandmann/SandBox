/*
 * ThreadReader.cpp
 *
 *  Created on: Jan 29, 2019
 *      Author: check
 */
#include "ThreadReader.h"
#include <thread>
#include <experimental/filesystem>

ClThreadReader::ClThreadReader()
{
 m_oPlayer.reset(new ClPlayer());
 m_oAudioDb.reset(new ClAudioDatabase());
 m_nReaderDelay = std::chrono::milliseconds(100);
 m_eCurrentStatus = EReaderStatus::EMPTY;
}

ClThreadReader::~ClThreadReader()
{

}

void ClThreadReader::read(StReaderMessage &stOutMessage)
{
	if (std::experimental::filesystem::exists("1")) {
		stOutMessage.eStatus = EReaderStatus::DETECTED;
		stOutMessage.nKey = 1;
	}
	else {
		stOutMessage.eStatus = EReaderStatus::EMPTY;
	}
}

void ClThreadReader::run()
{
	while(true)
	{
		StReaderMessage stMessage;
		read(stMessage);
		if (playbackNeeded(stMessage))
		{
			StAudioItem stAudioItem;
			if (m_oAudioDb->audioEntryFromKey(stMessage.nKey, stAudioItem)) {
				m_oPlayer->play(stAudioItem);
			}
		}
		else if (playbackToStop(stMessage))
		{
			m_oPlayer->stop();
		}
		std::this_thread::sleep_for(m_nReaderDelay);
	}
}


bool ClThreadReader::playbackNeeded(StReaderMessage &stMsg)
{
	return (m_eCurrentStatus == EReaderStatus::EMPTY && stMsg.eStatus == EReaderStatus::DETECTED);
}

bool ClThreadReader::playbackToStop(StReaderMessage &stMsg)
{
	return (m_eCurrentStatus == EReaderStatus::DETECTED && stMsg.eStatus == EReaderStatus::EMPTY);
}
