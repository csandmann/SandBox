/*
 * Manager.h
 *
 *  Created on: Jan 29, 2019
 *      Author: check
 */

#ifndef MANAGER_H_
#define MANAGER_H_

#include "PlayerPlugins/PlayerBase.h"
#include "ReaderPlugins/ReaderBase.h"

#include <thread>
#include <chrono>



class ClManager
{
public:
	ClManager(ClReaderBase *poReader);
	~ClManager();
	void start();
	void stop();
	void registerPlayer(ClPlayerBase *const poPlayer);
private:
	std::vector<ClPlayerBase*> m_vpPlayers;
	ClReaderBase *const m_poReader;

	ClPlayerBase *m_poActivePlayer;
	ReaderMessage::StMessage m_stCurrentMsg;

	bool m_bInterruptRequested;
	std::chrono::milliseconds m_nWaitTime;
	bool playbackNeeded(const ReaderMessage::StMessage &stMsg);
	bool playbackToStop(const ReaderMessage::StMessage &stMsg);
};




#endif /* MANAGER_H_ */
