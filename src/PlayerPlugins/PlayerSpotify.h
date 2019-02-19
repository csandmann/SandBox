/*
 * PlayerSpotify.h
 *
 *  Created on: Feb 18, 2019
 *      Author: check
 */

#ifndef SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_
#define SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_

#include "PlayerBase.h"
//cpprest
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class ClPlayerSpotify : public ClPlayerBase
{
public:
	ClPlayerSpotify();
	~ClPlayerSpotify() override;
	const std::string getIdentifier() const override;
	const bool restEndpointActive() const override;
	void play(const char* pcMessage) override;
	void stop() override;
	void pause() override;
	void increaseVolume() override;
	void decreaseVolume() override;
private:
	http_listener m_oSpotifyAuth;
	void handleSpotifyAuth(http_request oRequest);
};



#endif /* SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_ */
