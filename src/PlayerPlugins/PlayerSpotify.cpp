/*
 * PlayerSpotify.cpp
 *
 *  Created on: Feb 18, 2019
 *      Author: check
 */

#include "PlayerSpotify.h"
#include <sstream>
#include <fstream>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/uri_builder.h>

using namespace web::http::client;
using namespace web::json;

ClPlayerSpotify::ClPlayerSpotify(const StSpotifyConfig oConfig):
ClPlayerBase(&m_oConfig),
m_oSpotifyAuthReceiver(uri("http://localhost:8080/spotify/auth_receiver")),
m_oSpotifyMainSite(uri("http://localhost:8080/spotify")),
m_oLogger(ClLogger("Spotify")),
m_oConfig(oConfig),
m_sRedirectUri(buildRedirectUri()),
m_sSpotifyAuthorizationUri(buildSpotifyAuthorizationUri()),
m_oTokenFilePath(oConfig.oCacheDir / fs::path("spotifyTokens.txt")),
m_stTokens(SpotifyTokens::readTokens(m_oTokenFilePath.string().c_str()))
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

void ClPlayerSpotify::setTokensFromAuthCode(const char* pcAuthCode)
{
	m_oLogger.debug(std::string("tokensFromAuthCode: AuthCode=") + std::string(pcAuthCode));
	//build body of request
	uri_builder oParameters;
	oParameters.append_query(U("grant_type"), U("authorization_code"))
		.append_query(U("code"), U(pcAuthCode))
		.append_query(U("redirect_uri"), U(uri::encode_data_string(U(m_sRedirectUri))), false)
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
	            	this->m_oLogger.error("setTokensFromAuthCode: Received response " + response.to_string());
	            	return pplx::task_from_result(json::value());
	            };})
	        .then([this](pplx::task<json::value> previousTask){
	            try{
	                const json::value &oJson = previousTask.get();
					const auto oObject = oJson.as_object();
					//initialize tokens
					this->m_stTokens.bIsInitialized = true;
					this->m_stTokens.sAccessToken= oObject.at("access_token").as_string();
					this->m_stTokens.sRefreshToken = oObject.at("refresh_token").as_string();
	            	//dump tokens to file
					SpotifyTokens::dumpTokens(this->m_stTokens, m_oTokenFilePath.string().c_str());
	            }
	            catch(const http_exception &e){
	            	this->m_oLogger.error(std::string("setTokensFromAuthCode: Could not unpack JSON response: ") + std::string(e.what()));
	            }
	        });

	try{
	    oTask.wait();
	} catch(std::exception &e){
		m_oLogger.error(std::string("setTokensFromAuthCode: Error waiting for task: ") + std::string(e.what()));
	}

}

void ClPlayerSpotify::spotifyAuthReceiver(http_request oRequest)
{
	//Test-Calls
	//http://localhost:8080/spotify/auth_receiver?code=NApCCgBkWtQ&state=profile%2Factivity
	//http://localhost:8080/spotify/auth_receiver?error=access_denied&state=STATE
	m_oLogger.debug(std::string("spotifyAuthReceiver: Incoming uri ") + oRequest.request_uri().to_string());
	auto oRequestArgs = uri::split_query(uri::decode(oRequest.request_uri().query()));
	if (oRequestArgs.find("code") != oRequestArgs.end())
	{
		setTokensFromAuthCode(oRequestArgs.at("code").c_str());
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

std::string ClPlayerSpotify::buildRedirectUri()
{
	//build redirect URI
	uri_builder oRedirectUri;
	oRedirectUri.set_scheme("http");
	oRedirectUri.set_host(m_oConfig.sHostname);
	oRedirectUri.set_port(m_oConfig.nPort);
	oRedirectUri.set_path(m_oSpotifyAuthReceiver.uri().path());
	return oRedirectUri.to_string();
}

std::string ClPlayerSpotify::buildSpotifyAuthorizationUri()
{
	//build uri
	std::string sRedirectUri = uri::encode_data_string(m_sRedirectUri);
	uri_builder oSpotifyAuth(U("https://accounts.spotify.com/authorize"));
	oSpotifyAuth
		.append_query(U("client_id"), U(m_oConfig.sClientId))
		.append_query(U("response_type"), U("code"))
		.append_query(U("redirect_uri"), U(sRedirectUri), false);
	std::string sAuthUri = oSpotifyAuth.to_string();
	return sAuthUri;
}


bool SpotifyTokens::dumpTokens(const StTokens &stTokens, const char* pcOutputFile)
{
	try
	{
		const auto oBaseDir = fs::path(pcOutputFile).parent_path();
		if (!fs::exists(oBaseDir)) {
			fs::create_directories(oBaseDir);
		}
		FILE* pfOut = fopen(pcOutputFile, "w");
		fprintf(pfOut, "%s\n%s", stTokens.sAccessToken.c_str(), stTokens.sRefreshToken.c_str());
		fclose(pfOut);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

SpotifyTokens::StTokens SpotifyTokens::readTokens(const char* pcInputFile)
{
	StTokens stTokens;
	try
	{
		std::ifstream ifsIn(pcInputFile);
		std::getline(ifsIn, stTokens.sAccessToken);
		std::getline(ifsIn, stTokens.sRefreshToken);
		ifsIn.close();
		stTokens.bIsInitialized = true;
	}
	catch (...)
	{
		stTokens.bIsInitialized = false;
	}
	return stTokens;
}
