/*
 * Manager.h
 *
 *  Created on: Jan 29, 2019
 *      Author: check
 */

#ifndef MANAGER_H_
#define MANAGER_H_

#include <thread>
#include <chrono>

#include "PlayerPlugins/PlayerBase.h"
#include "ReaderPlugins/ReaderBase.h"
#include "AudioDatabase.h"


class ClManager
{
public:
	ClManager(ClReaderBase *const poReader, ClAudioDatabase *const poAudioDb);
	~ClManager();
	void start();
	void stop();
	void registerPlayer(ClPlayerBase *const poPlayer);
private:
	std::vector<ClPlayerBase*> m_vpPlayers;
	ClReaderBase *const m_poReader;
	ClAudioDatabase *const m_poAudioDb;

	ClPlayerBase *m_poActivePlayer;
	StReaderMessage m_stCurrentMsg;

	bool m_bInterruptRequested;
	std::chrono::milliseconds m_nWaitTime;
	bool playbackNeeded(const StReaderMessage &stMsg);
	bool playbackToStop(const StReaderMessage &stMsg);
};




#endif /* MANAGER_H_ */