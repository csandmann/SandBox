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
	ClManager(const ClReaderBase *poReader, const ClAudioDatabase *poAudioDb);
	~ClManager();
	void start();
	void stop();
	void registerPlugin(const ClPlayerBase *poPlayer);
private:
	std::vector<ClPlayerBase*> m_vpPlayers;
	ClReaderBase *m_poReader;
	ClAudioDatabase *m_poAudioDb;

	StReaderMessage m_stCurrentMsg;
	bool m_bInterruptRequested;
	std::chrono::milliseconds m_nWaitTime;

	bool playbackNeeded(const StReaderMessage &stMsg);
	bool playbackToStop(const StReaderMessage &stMsg);
};




#endif /* MANAGER_H_ */
