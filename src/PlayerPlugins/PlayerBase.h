/*
 * PlayerBase.h
 *
 *  Created on: Jan 29, 2019
 *      Author: check
 */

#ifndef PLAYERBASE_H_
#define PLAYERBASE_H_

#include <string>
#include <vector>

struct StPlayerConfig{};


class ClPlayerBase
{
public:
	ClPlayerBase(const StPlayerConfig *poPlayerConfig);
	virtual ~ClPlayerBase(){};
	virtual const std::string getIdentifier() const = 0;
	virtual const bool restEndpointActive() const = 0;
	virtual void execute(const std::vector<unsigned char> &vcMessage) = 0;
	virtual void stop() = 0;
	virtual void pause() = 0;
    virtual void resume() = 0;
	virtual void increaseVolume() = 0;
	virtual void decreaseVolume() = 0;
    virtual std::vector<unsigned char> getMessageToWrite() = 0;
protected:
	const StPlayerConfig *m_poBaseConfig;
};

inline
ClPlayerBase::ClPlayerBase(const StPlayerConfig *poPlayerConfig) :
m_poBaseConfig(poPlayerConfig)
{}

#endif /* PLAYERBASE_H_ */
