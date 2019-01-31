/*
 * ThreadReader.h
 *
 *  Created on: Jan 29, 2019
 *      Author: check
 */

#ifndef THREADREADER_H_
#define THREADREADER_H_

#include <chrono>
#include <string>
#include <thread>

#include "Player.h"
#include "AudioDatabase.h"


enum EReaderStatus : int
{
	DETECTED = 0,
	EMPTY = 1
};

struct StReaderMessage
{
	EReaderStatus eStatus;
	int nKey;
};

class ClThreadReader
{
public:
	ClThreadReader();
	~ClThreadReader();
	void run();
private:
	void read(StReaderMessage &stOutMessage);
	std::unique_ptr<ClPlayer> m_oPlayer;
	std::unique_ptr<ClAudioDatabase> m_oAudioDb;

	std::chrono::milliseconds m_nReaderDelay;
	StReaderMessage m_eCurrentStatus;

	bool playbackNeeded(StReaderMessage &stMsg);
	bool playbackToStop(StReaderMessage &stMsg);
};




#endif /* THREADREADER_H_ */
