/*
 * PlayerSpotify.cpp
 *
 *  Created on: Feb 18, 2019
 *      Author: check
 */

#include "PlayerSpotify.h"
#include <sstream>

#define S2U(x) utility::conversions::to_string_t(x)
#define U2S(x) utility::conversions::to_utf8string(x)

using namespace web::http::client;
using namespace web::json;

ClPlayerSpotify::ClPlayerSpotify(const StSpotifyConfig oConfig):
ClPlayerBase(&m_oConfig),
m_oLogger(ClLogger("Spotify")),
m_oConfig(oConfig),
m_oSpotifyAuthCodeReceiver(uri(U("http://localhost:8080/spotify/auth_receiver"))),
m_oSpotifyMainSite(uri(U("http://localhost:8080/spotify"))),
m_oTokenFilePath(oConfig.oCacheDir / fs::path("spotifyTokens.txt")),
m_stTokens(SpotifyTokens::readTokens(m_oTokenFilePath.string()))
{
	//start listeners for websites
	m_oSpotifyAuthCodeReceiver.open().wait();
	m_oSpotifyAuthCodeReceiver.support(methods::GET,  [this](http_request request){ this->cbkSpotifyAuthCodeReceiver(request); });
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

SpotifyTokens::StTokens ClPlayerSpotify::getTokensFromAuthCode(const utility::string_t &sAuthCode)
{
	m_oLogger.debug(std::string("getTokensFromAuthCode: AuthCode=") + U2S(sAuthCode));
	SpotifyTokens::StTokens stTokens;
	//build body of request
	uri_builder oParameters;
	oParameters.append_query(U("grant_type"), U("authorization_code"))
		.append_query(U("code"), sAuthCode)
		.append_query(U("redirect_uri"), uri::encode_data_string(buildRedirectUri()), false)
		.append_query(U("client_id"), S2U(m_oConfig.sClientId))
		.append_query(U("client_secret"), S2U(m_oConfig.sClientSecret));
	auto sParameters = oParameters.to_string().substr(2);
	//build request
	http_request oRequest(methods::POST);
	oRequest.headers().add(U("Content-Type"), U("application/x-www-form-urlencoded"));
	oRequest.set_body(S2U(sParameters));
	//make request
	http_client oClient(U("https://accounts.spotify.com/api/token"));
	pplx::task<void> oTask = oClient.request(oRequest)
	        .then([this](http_response response)-> pplx::task<json::value>{
	            if(response.status_code() == status_codes::OK){
	                return response.extract_json();
	            } else {
	            	this->m_oLogger.error(std::string("setTokensFromAuthCode: Could not parse response ") + U2S(response.to_string()));
	            	return pplx::task_from_result(json::value());
	            };})
	        .then([&](pplx::task<json::value> previousTask){
	            try{
	                const json::value &oJson = previousTask.get();
					const auto oObject = oJson.as_object();
					//initialize tokens
					stTokens.bIsInitialized = true;
					stTokens.sAccessToken = U2S(oObject.at(U("access_token")).as_string());
					stTokens.sRefreshToken = U2S(oObject.at(U("refresh_token")).as_string());
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

void ClPlayerSpotify::cbkSpotifyAuthCodeReceiver(http_request oRequest)
{
	//Test-Calls
	//http://localhost:8080/spotify/auth_receiver?code=NApCCgBkWtQ&state=profile%2Factivity
	//http://localhost:8080/spotify/auth_receiver?error=access_denied&state=STATE
	//functionality
	m_oLogger.debug(std::string("spotifyAuthReceiver: Incoming uri ") + U2S(oRequest.request_uri().to_string()));
	//get auth code from incomming uri
	auto oRequestArgs = uri::split_query(uri::decode(oRequest.request_uri().query()));
	if (oRequestArgs.find(U("code")) != oRequestArgs.end())
	{
		m_stTokens = getTokensFromAuthCode(oRequestArgs.at(U("code")));
    	//dump tokens to file
		SpotifyTokens::dumpTokens(m_stTokens, m_oTokenFilePath.string());
	}
	else if (oRequestArgs.find(U("error")) != oRequestArgs.end())
	{
		m_stTokens.bIsInitialized = false;
		m_oLogger.error(std::string("spotifyAuthReceiver ") + U2S(oRequestArgs.at(U("error"))));
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
	refreshAccessToken();
	http_response oResponse(status_codes::OK);
	oResponse.headers().add(U("Content-Type"), U("text/html"));
	std::stringstream ss;
	ss << "<html> <head></head><body><a href=\"" << U2S(buildSpotifyAuthorizationUri()) << "\"> Connect Account </a> </body> </html>";
	oResponse.set_body(ss.str());
	oRequest.reply(oResponse);
}

const utility::string_t ClPlayerSpotify::buildRedirectUri()
{
	//build redirect URI
	uri_builder oRedirectUri;
	oRedirectUri.set_scheme(U("http"))
		.set_host(S2U(m_oConfig.sHostname))
		.set_port(m_oConfig.nPort)
		.set_path(m_oSpotifyAuthCodeReceiver.uri().path());
	return oRedirectUri.to_string();
}

const utility::string_t ClPlayerSpotify::buildSpotifyAuthorizationUri()
{
	//build uri
	auto sRedirectUri = uri::encode_data_string(buildRedirectUri());
	auto sScope = uri::encode_data_string(U("user-read-playback-state user-modify-playback-state"));
	uri_builder oSpotifyAuth(U("https://accounts.spotify.com/authorize"));
	oSpotifyAuth
		.append_query(U("client_id"), S2U(m_oConfig.sClientId))
		.append_query(U("response_type"), U("code"))
		.append_query(U("redirect_uri"), sRedirectUri, false)
		.append_query(U("scope"), sScope, false);
	auto sAuthUri = oSpotifyAuth.to_string();
	return sAuthUri;
}

void ClPlayerSpotify::refreshAccessToken()
{
	//build body of request
	uri_builder oParameters;
	oParameters.append_query(U("grant_type"), U("refresh_token"))
			   .append_query(U("refresh_token"), S2U(m_stTokens.sRefreshToken));
	auto sParameters = oParameters.to_string().substr(2);
	//get base64-encoded authorization
	auto sAuthorization = utility::conversions::to_utf8string(m_oConfig.sClientId + std::string(":") + m_oConfig.sClientSecret);
	std::vector<unsigned char> vcAuthorization(sAuthorization.size());
	std::memcpy(vcAuthorization.data(), sAuthorization.c_str(), sAuthorization.size());
	auto sEncodedAuth = utility::conversions::to_base64(vcAuthorization);
	//build request
	http_request oRequest(methods::POST);
	oRequest.headers().add(U("Content-Type"), U("application/x-www-form-urlencoded"));
	oRequest.headers().add(U("Authorization"), sEncodedAuth);
	oRequest.set_body(sParameters);
	//make request
	http_client oClient(U("https://accounts.spotify.com/api/token"));
	pplx::task<void> oTask = oClient.request(oRequest)
	        .then([this](http_response response)-> pplx::task<json::value>{
	            if(response.status_code() == status_codes::OK){
	                return response.extract_json();
	            } else {
	            	this->m_oLogger.error("refreshAccessToken: Could not parse response " + U2S(response.to_string()));
	            	return pplx::task_from_result(json::value());
	            };})
	        .then([&](pplx::task<json::value> previousTask){
	            try{
	                const json::value &oJson = previousTask.get();
					const auto oObject = oJson.as_object();
					//initialize tokens
					this->m_stTokens.sAccessToken= U2S(oObject.at(U("access_token")).as_string());
	            }
	            catch(const http_exception &e){
	            	this->m_oLogger.error(std::string("refreshAccessToken: Could not unpack JSON response: ") + std::string(e.what()));
	            }
	        });

	try{
	    oTask.wait();
	} catch(std::exception &e){
		m_oLogger.error(std::string("refreshAccessToken: Could not perform request: ") + std::string(e.what()));
	}
}
