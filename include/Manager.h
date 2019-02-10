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

#include "Player.h"
#include "ReaderBase.h"
#include "AudioDatabase.h"


class ClManager
{
public:
	ClManager(const std::shared_ptr<ClPlayer> &spPlayer, const std::shared_ptr<ClReaderBase> &spReader, const std::shared_ptr<ClAudioDatabase> &spAudioDb);
	~ClManager();
	void start();
	void stop();
private:
	std::shared_ptr<ClPlayer> m_spPlayer;
	std::shared_ptr<ClReaderBase> m_spReader;
	std::shared_ptr<ClAudioDatabase> m_spAudioDb;

	StReaderMessage m_stCurrentMsg;
	bool m_bInterruptRequested;
	std::chrono::milliseconds m_nWaitTime;

	bool playbackNeeded(const StReaderMessage &stMsg);
	bool playbackToStop(const StReaderMessage &stMsg);
};




#endif /* MANAGER_H_ */
