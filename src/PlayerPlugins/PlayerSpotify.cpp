/*
 * PlayerSpotify.cpp
 *
 *  Created on: Feb 18, 2019
 *      Author: check
 */

#include "PlayerSpotify.h"
#include <sstream>
#include <regex>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/uri_builder.h>

using namespace web::http::client;

ClPlayerSpotify::ClPlayerSpotify(const StSpotifyConfig oConfig):
ClPlayerBase(&m_oConfig),
m_oSpotifyAuthReceiver(uri("http://localhost:8080/spotify/auth_receiver")),
m_oSpotifyMainSite(uri("http://localhost:8080/spotify")),
m_oLogger(ClLogger("Spotify")),
m_oConfig(oConfig),
m_sSpotifyAuthorizationUri(buildSpotifyAuthorizationUri())
{
	m_oSpotifyAuthReceiver.open().wait();
	m_oSpotifyAuthReceiver.support(methods::GET,  [this](http_request request){ this->spotifyAuthReceiver(request); });
	m_oSpotifyMainSite.open().wait();
	m_oSpotifyMainSite.support(methods::GET,  [this](http_request request){ this->spotifyMainSite(request); });
}

ClPlayerSpotify::~ClPlayerSpotify() {
}

const std::string ClPlayerSpotify::getIdentifier() const {
	return "spotify";
}

const bool ClPlayerSpotify::restEndpointActive() const {
	return true;
}

void ClPlayerSpotify::play(const char* pcMessage)
{
	m_oLogger.info(std::string("Playing ") + std::string(pcMessage));
}

void ClPlayerSpotify::stop()
{
	m_oLogger.info("Stopped");
}

void ClPlayerSpotify::pause()
{
	m_oLogger.info("Paused");
}

void ClPlayerSpotify::increaseVolume()
{
	m_oLogger.info("Increasing volume");
}

void ClPlayerSpotify::decreaseVolume()
{
	m_oLogger.info("Decreasing volume");
}

StSpotifyTokens ClPlayerSpotify::tokensFromAuthCode(const StSpotifyAuthCode &stAuthCode)
{
	StSpotifyTokens stTokens;
	if (!stAuthCode.bIsInitialized) {
		return stTokens;
	}

//	std::string sData;
//	http_client oClient;
//	oClient.request(methods::POST, "https://accounts.spotify.com/api/token", sData);
	return stTokens;
}

void ClPlayerSpotify::spotifyAuthReceiver(http_request oRequest)
{
	m_oLogger.debug(std::string("spotifyAuthReceiver: Incoming uri ") + oRequest.request_uri().to_string());
	//get auth_code from request uri
	//http://localhost:8080/spotify/auth_receiver?code=NApCCgBkWtQ&state=profile%2Factivity
	//http://localhost:8080/spotify/auth_receiver?error=access_denied&state=STATE

	StSpotifyAuthCode stAuthCode;
	auto oRequestArgs = uri::split_query(uri::decode(oRequest.request_uri().query()));
	if (oRequestArgs.find("code") != oRequestArgs.end())
	{
		stAuthCode.bIsInitialized = true;
		stAuthCode.sAuthCode = oRequestArgs.at("code");
	}
	else if (oRequestArgs.find("error") != oRequestArgs.end())
	{
		stAuthCode.bIsInitialized = false;
		m_oLogger.error(std::string("spotifyAuthReceiver " + oRequestArgs.at("error"));
	}
	else
	{
		m_oLogger.error(std::string("spotifyAuthReceiver: Internal error. Has spotify changed it's authorization handshake?"));
	}

	StSpotifyTokens stSpotifyTokens;

	http_response oResponse(status_codes::OK);
	oResponse.headers().add(U("Content-Type"), U("text/html"));
	if (!stAuthCode.bIsInitialized || !stSpotifyTokens.bIsInitialized)
	{
		oResponse.set_body("<html><head></head><body>Error (see log)!</body></html>");
	}
	else
	{
		oResponse.set_body("<html><head></head><body>Success!</body></html>");
	}
	oRequest.reply(oResponse);
}

std::vector<std::string> ClPlayerSpotify::splitStringAtDelimiter(const std::string &sInput, const char cDelimiter)
{
	std::stringstream ss(sInput);
	std::vector<std::string> vsSplitString;
	std::string item;
	while (std::getline(ss, item, cDelimiter))
	{
		vsSplitString.push_back(item);
	}
	return vsSplitString;
}

void ClPlayerSpotify::spotifyMainSite(http_request oRequest)
{
	//send Website response
	http_response oResponse(status_codes::OK);
	oResponse.headers().add(U("Content-Type"), U("text/html"));
	std::stringstream ss;
	ss << "<html> <head></head><body><a href=\"" << m_sSpotifyAuthorizationUri << "\"> Connect Account </a> </body> </html>";
	oResponse.set_body(ss.str());
	oRequest.reply(oResponse);
}

std::string ClPlayerSpotify::buildSpotifyAuthorizationUri()
{
	//build local URI
	uri_builder oRedirectUri;
	oRedirectUri.set_scheme("http");
	oRedirectUri.set_host(m_oConfig.sHostname);
	oRedirectUri.set_port(m_oConfig.nPort);
	oRedirectUri.set_path(m_oSpotifyAuthReceiver.uri().path());
	std::string sRedirectUri = uri::encode_data_string(oRedirectUri.to_string());

	uri_builder oSpotifyAuth(U("https://accounts.spotify.com/authorize"));
	oSpotifyAuth
		.append_query(U("client_id"), U(m_oConfig.sClientId))
		.append_query(U("response_type"), U("code"))
		.append_query(U("redirect_uri"), U(sRedirectUri), false);
	return oSpotifyAuth.to_string();
}
