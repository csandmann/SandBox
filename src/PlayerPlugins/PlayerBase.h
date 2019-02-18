/*
 * PlayerBase.h
 *
 *  Created on: Jan 29, 2019
 *      Author: check
 */

#ifndef PLAYERBASE_H_
#define PLAYERBASE_H_

#include <string>
#include "../AudioDatabase.h"

class ClPlayerBase
{
public:
	ClPlayerBase();
	virtual ~ClPlayerBase();
	virtual const std::string getIdentifier() const;
	virtual const bool restEndpointActive() const;
	virtual void play(const char* pcMessage);
	virtual void stop();
	virtual void pause();
	virtual void increaseVolume();
	virtual void decreaseVolume();
};

#endif /* PLAYERBASE_H_ */
