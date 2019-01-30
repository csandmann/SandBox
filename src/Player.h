/*
 * ThreadPlayer.h
 *
 *  Created on: Jan 29, 2019
 *      Author: check
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include "AudioDatabase.h"

class ClPlayer
{
public:
	ClPlayer();
	~ClPlayer();
	void play(StAudioItem &stMsg);
	void stop();
	void pause();
private:
	void playSpotify(std::string sMessage);
};

#endif /* PLAYER_H_ */
