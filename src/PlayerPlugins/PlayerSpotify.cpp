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

StSpotifyAuthCode ClPlayerSpotify::authCodeFromUri(const std::string &sUri)
{
	StSpotifyAuthCode stAuthCode;
	auto vsSplitUri = splitStringAtDelimiter(sUri, '?');
	if (vsSplitUri.size() != 2) //major problem
	{
		auto sMessage = std::string("authCodeFromUri: Unknown response from spotify ") + sUri;
		m_oLogger.error(sMessage);
		stAuthCode.bIsInitialized = false;
	}
	else //everything OK
	{
		auto vsArguments = splitStringAtDelimiter(vsSplitUri[1], '&');
		for (auto &sArg : vsArguments)
		{
			auto vsTuple = splitStringAtDelimiter(sArg, '=');
			if (vsTuple.size() > 1 && vsTuple[0] == "error")
			{
				auto sMessage = std::string("authCodeFromUri: Received error from spotify: ") + vsTuple[1];
				m_oLogger.error(sMessage);
				stAuthCode.bIsInitialized = false;
				break;
			}
			else if (vsTuple.size() > 1 && vsTuple[0] == "code") {
				stAuthCode.sAuthCode = vsTuple[1];
				stAuthCode.bIsInitialized = true;
				break;
			}
		}
	}
	return stAuthCode;
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
	auto stAuthCode = authCodeFromUri(oRequest.request_uri().to_string());
	auto stSpotifyTokens = tokensFromAuthCode(stAuthCode);
	//send OK response
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

	uri_builder oSpotifyAuth(U("https://accounts.spotify.com/authorize")) \
	.append_query(U("client_id"), U(m_oConfig.sClientId)) \
	.append_query(U("response_type"), U("code")) \
	.append_query(U("redirect_uri"), U(sRedirectUri), false);
	std::string sTest = oSpotifyAuth.to_string();
	return sTest;
}
