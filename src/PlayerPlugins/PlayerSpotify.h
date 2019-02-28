/*
 * PlayerSpotify.h
 *
 *  Created on: Feb 18, 2019
 *      Author: check
 */

#ifndef SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_
#define SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_

#include "PlayerBase.h"
#include "../Logging/Logger.h"
//cpprest
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

struct StSpotifyConfig : StPlayerConfig
{
	std::string sClientId;
	std::string sClientSecret;
	std::string sHostname;
	unsigned int nPort;
};

struct StSpotifyAuthCode
{
	bool bIsInitialized = false;
	std::string sAuthCode;
};

struct StSpotifyTokens
{
	bool bIsInitialized = false;
	std::string sAccessToken;
	std::string sRefreshToken;
};


class ClPlayerSpotify : public ClPlayerBase
{
public:
	ClPlayerSpotify(const StSpotifyConfig oConfig);
	~ClPlayerSpotify() override;
	const std::string getIdentifier() const override;
	const bool restEndpointActive() const override;
	void play(const char* pcMessage) override;
	void stop() override;
	void pause() override;
	void increaseVolume() override;
	void decreaseVolume() override;
private:
	http_listener m_oSpotifyAuthReceiver;
	http_listener m_oSpotifyMainSite;
	ClLogger m_oLogger;
	const StSpotifyConfig m_oConfig;
	const std::string m_sSpotifyAuthorizationUri;
	StSpotifyTokens m_stTokens;

	void spotifyAuthReceiver(http_request oRequest);
	void spotifyAuth(http_request oRequest);
	void spotifyMainSite(http_request oRequest);
	std::string buildSpotifyAuthorizationUri();
	StSpotifyTokens tokensFromAuthCode(const StSpotifyAuthCode &stAuthCode);
	std::string encodeClientIdAndSecret();
	std::vector<std::string> splitStringAtDelimiter(const std::string &sInput, const char cDelimiter);
};



#endif /* SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_ */
