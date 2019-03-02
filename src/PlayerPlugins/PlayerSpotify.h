/*
 * PlayerSpotify.h
 *
 *  Created on: Feb 18, 2019
 *      Author: check
 */

#ifndef SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_
#define SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_

#include <experimental/filesystem>

#include "PlayerBase.h"
#include "../Logging/Logger.h"
//cpprest
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
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

namespace SpotifyTokens
{
	struct StTokens
	{
		bool bIsInitialized = false;
		std::string sAccessToken;
		std::string sRefreshToken;
	};

	bool dumpTokens(const StTokens &stTokens, const char* pcOutputFile);
	StTokens readTokens(const char* pcInputFile);
}


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
	const std::string m_sRedirectUri;
	const std::string m_sSpotifyAuthorizationUri;
	fs::path m_oTokenFilePath;
	SpotifyTokens::StTokens m_stTokens;

	//http handlers
	void spotifyAuthReceiver(http_request oRequest);
	void spotifyAuth(http_request oRequest);
	void spotifyMainSite(http_request oRequest);
	//helper functions
	std::string buildRedirectUri();
	std::string buildSpotifyAuthorizationUri();
	void setTokensFromAuthCode(const char* stAuthCode);
	void dumpTokens();
	void readTokens();
};



#endif /* SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_ */
