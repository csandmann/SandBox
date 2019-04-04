/*
 * SpotifyAuthorizationModule.h
 *
 *  Created on: Mar 4, 2019
 *      Author: check
 */

#ifndef SRC_PLAYERPLUGINS_PLAYERSPOTIFY_SPOTIFYAUTHORIZATIONMODULE_H_
#define SRC_PLAYERPLUGINS_PLAYERSPOTIFY_SPOTIFYAUTHORIZATIONMODULE_H_

#include "../../Logging/Logger.h"
#include "SpotifyTokens.h"
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/uri_builder.h>
#include <boost/format.hpp>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::experimental::listener;

#define S2U(x) utility::conversions::to_string_t(x)
#define U2S(x) utility::conversions::to_utf8string(x)

struct StSpotifyAuthorizationConfig
{
	std::string sClientId;
	std::string sClientSecret;
	std::string sHostname;
	int nPort;
	std::string sTokenFilePath;
};

class ClSpotifyAuthorizationModule
{
public:
	ClSpotifyAuthorizationModule(const StSpotifyAuthorizationConfig m_oConfig);
	void refreshAccessToken();
	std::string getAccessToken();
	bool isInitialized();
	const utility::string_t getSpotifyAuthorizationUri();

private:
	//members
	ClLogger m_oLogger;
	StSpotifyAuthorizationConfig m_oConfig;
	http_listener m_oSpotifyAuthCodeReceiver;
	SpotifyTokens::StTokens m_stTokens; 
	//functions
	void getTokensFromAuthCode(const utility::string_t &sAuthCode);
	void cbkSpotifyAuthCodeReceiver(http_request oRequest);
	const utility::string_t buildRedirectUri();

};


#endif /*SRC_PLAYERPLUGINS_PLAYERSPOTIFY_SPOTIFYAUTHORIZATIONMODULE_H_*/
