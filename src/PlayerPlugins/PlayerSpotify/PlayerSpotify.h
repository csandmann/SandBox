/*
 * PlayerSpotify.h
 *
 *  Created on: Feb 18, 2019
 *      Author: check
 */

#ifndef SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_
#define SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_

#include "../PlayerBase.h"
#include "../../Logging/Logger.h"
#include "SpotifyTokens.h"
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <experimental/filesystem>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
namespace fs = std::experimental::filesystem;


struct StSpotifyConfig : StPlayerConfig
{
	std::string sClientId;
	std::string sClientSecret;
	std::string sHostname;
	unsigned int nPort;
	fs::path oCacheDir;
};


class ClPlayerSpotify : public ClPlayerBase
{
public:
	ClPlayerSpotify(const StSpotifyConfig oConfig);
	~ClPlayerSpotify() override;
	const std::string getIdentifier() const override;
	const bool restEndpointActive() const override;
	void play(const std::string &Message) override;
	void stop() override;
	void pause() override;
	void increaseVolume() override;
	void decreaseVolume() override;
private:
	//main functionality
	ClLogger m_oLogger;
	const StSpotifyConfig m_oConfig;
	//listeners
	http_listener m_oSpotifyAuthReceiver;
	http_listener m_oSpotifyMainSite;
	//tokens
	fs::path m_oTokenFilePath;
	SpotifyTokens::StTokens m_stTokens;
	//http callbacks
	void cbkSpotifyAuthReceiver(http_request oRequest);
	void cbkSpotifyMainSite(http_request oRequest);
	//helper functions
	const std::string buildRedirectUri() const;
	const std::string buildSpotifyAuthorizationUri() const;
	SpotifyTokens::StTokens getTokensFromAuthCode(const std::string &sAuthCode);
};



#endif /* SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_ */
