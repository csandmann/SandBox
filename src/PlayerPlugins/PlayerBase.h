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
#include "../Configuration.h"

class ClPlayerBase
{
public:
	ClPlayerBase(const ClConfiguration &oConfig);
	virtual ~ClPlayerBase(){};
	virtual const std::string getIdentifier() const = 0;
	virtual const bool restEndpointActive() const = 0;
	virtual void play(const char* pcMessage) = 0;
	virtual void stop() = 0;
	virtual void pause() = 0;
	virtual void increaseVolume() = 0;
	virtual void decreaseVolume() = 0;
protected:
	const ClConfiguration &m_oConfiguration;
};

inline
ClPlayerBase::ClPlayerBase(const ClConfiguration &oConfig) :
m_oConfiguration(oConfig)
{}

#endif /* PLAYERBASE_H_ */
