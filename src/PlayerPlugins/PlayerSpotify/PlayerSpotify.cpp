/*
 * PlayerSpotify.cpp
 *
 *  Created on: Feb 18, 2019
 *      Author: check
 */

#include "PlayerSpotify.h"
#include <sstream>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/uri_builder.h>

using namespace web::http::client;
using namespace web::json;

ClPlayerSpotify::ClPlayerSpotify(const StSpotifyConfig oConfig):
ClPlayerBase(&m_oConfig),
m_oLogger(ClLogger("Spotify")),
m_oConfig(oConfig),
m_oSpotifyAuthReceiver(uri("http://localhost:8080/spotify/auth_receiver")),
m_oSpotifyMainSite(uri("http://localhost:8080/spotify")),
m_oTokenFilePath(oConfig.oCacheDir / fs::path("spotifyTokens.txt")),
m_stTokens(SpotifyTokens::readTokens(m_oTokenFilePath.string()))
{
	m_oSpotifyAuthReceiver.open().wait();
	m_oSpotifyAuthReceiver.support(methods::GET,  [this](http_request request){ this->cbkSpotifyAuthReceiver(request); });
	m_oSpotifyMainSite.open().wait();
	m_oSpotifyMainSite.support(methods::GET,  [this](http_request request){ this->cbkSpotifyMainSite(request); });
}

ClPlayerSpotify::~ClPlayerSpotify() {
}

const std::string ClPlayerSpotify::getIdentifier() const {
	return "spotify";
}

const bool ClPlayerSpotify::restEndpointActive() const {
	return true;
}

void ClPlayerSpotify::play(const std::string &sMessage)
{
	m_oLogger.info(std::string("Playing ") + sMessage);
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

SpotifyTokens::StTokens ClPlayerSpotify::getTokensFromAuthCode(const std::string &sAuthCode)
{
	m_oLogger.debug(std::string("getTokensFromAuthCode: AuthCode=") + sAuthCode);
	SpotifyTokens::StTokens stTokens;
	//build body of request
	uri_builder oParameters;
	oParameters.append_query(U("grant_type"), U("authorization_code"))
		.append_query(U("code"), U(sAuthCode))
		.append_query(U("redirect_uri"), U(uri::encode_data_string(U(buildRedirectUri()))), false)
		.append_query(U("client_id"), U(m_oConfig.sClientId))
		.append_query(U("client_secret"), U(m_oConfig.sClientSecret));
	std::string sParameters = oParameters.to_string().substr(2);
	//build request
	http_request oRequest(methods::POST);
	oRequest.headers().add(U("Content-Type"), U("application/x-www-form-urlencoded"));
	oRequest.set_body(U(sParameters));
	//make request
	http_client oClient("https://accounts.spotify.com/api/token");
	pplx::task<void> oTask = oClient.request(oRequest)
	        .then([this](http_response response)-> pplx::task<json::value>{
	            if(response.status_code() == status_codes::OK){
	                return response.extract_json();
	            } else {
	            	this->m_oLogger.error("setTokensFromAuthCode: Could not parse response " + response.to_string());
	            	return pplx::task_from_result(json::value());
	            };})
	        .then([&](pplx::task<json::value> previousTask){
	            try{
	                const json::value &oJson = previousTask.get();
					const auto oObject = oJson.as_object();
					//initialize tokens
					stTokens.bIsInitialized = true;
					stTokens.sAccessToken= oObject.at("access_token").as_string();
					stTokens.sRefreshToken = oObject.at("refresh_token").as_string();
	            }
	            catch(const http_exception &e){
	            	this->m_oLogger.error(std::string("getTokensFromAuthCode: Could not unpack JSON response: ") + std::string(e.what()));
	            }
	        });

	try{
	    oTask.wait();
	} catch(std::exception &e){
		m_oLogger.error(std::string("getTokensFromAuthCode: Could not perform request: ") + std::string(e.what()));
	}
	return stTokens;
}

void ClPlayerSpotify::cbkSpotifyAuthReceiver(http_request oRequest)
{
	//Test-Calls
	//http://localhost:8080/spotify/auth_receiver?code=NApCCgBkWtQ&state=profile%2Factivity
	//http://localhost:8080/spotify/auth_receiver?error=access_denied&state=STATE
	//functionality
	m_oLogger.debug(std::string("spotifyAuthReceiver: Incoming uri ") + oRequest.request_uri().to_string());
	//get auth code from incomming uri
	auto oRequestArgs = uri::split_query(uri::decode(oRequest.request_uri().query()));
	if (oRequestArgs.find("code") != oRequestArgs.end())
	{
		m_stTokens = getTokensFromAuthCode(oRequestArgs.at("code"));
    	//dump tokens to file
		SpotifyTokens::dumpTokens(m_stTokens, m_oTokenFilePath.string());
	}
	else if (oRequestArgs.find("error") != oRequestArgs.end())
	{
		m_stTokens.bIsInitialized = false;
		m_oLogger.error(std::string("spotifyAuthReceiver ") + oRequestArgs.at("error"));
	}
	else
	{
		m_oLogger.error(std::string("spotifyAuthReceiver: Internal error. Has spotify changed it's authorization handshake?"));
	}
	//send response
	http_response oResponse(status_codes::OK);
	oResponse.headers().add(U("Content-Type"), U("text/html"));
	if (!m_stTokens.bIsInitialized)
	{
		oResponse.set_body("<html><head></head><body>Error (see log)!</body></html>");
	}
	else
	{
		oResponse.set_body("<html><head></head><body>Success!</body></html>");
	}
	oRequest.reply(oResponse);
}

void ClPlayerSpotify::cbkSpotifyMainSite(http_request oRequest)
{
	//send Website response
	http_response oResponse(status_codes::OK);
	oResponse.headers().add(U("Content-Type"), U("text/html"));
	std::stringstream ss;
	ss << "<html> <head></head><body><a href=\"" << buildSpotifyAuthorizationUri() << "\"> Connect Account </a> </body> </html>";
	oResponse.set_body(ss.str());
	oRequest.reply(oResponse);
}

const std::string ClPlayerSpotify::buildRedirectUri() const
{
	//build redirect URI
	uri_builder oRedirectUri;
	oRedirectUri.set_scheme("http")
		.set_host(m_oConfig.sHostname)
		.set_port(m_oConfig.nPort)
		.set_path(m_oSpotifyAuthReceiver.uri().path());
	return oRedirectUri.to_string();
}

const std::string ClPlayerSpotify::buildSpotifyAuthorizationUri() const
{
	//build uri
	std::string sRedirectUri = uri::encode_data_string(buildRedirectUri());
	uri_builder oSpotifyAuth(U("https://accounts.spotify.com/authorize"));
	oSpotifyAuth
		.append_query(U("client_id"), U(m_oConfig.sClientId))
		.append_query(U("response_type"), U("code"))
		.append_query(U("redirect_uri"), U(sRedirectUri), false);
	std::string sAuthUri = oSpotifyAuth.to_string();
	return sAuthUri;
}
