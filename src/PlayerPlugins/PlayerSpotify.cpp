/*
 * PlayerSpotify.cpp
 *
 *  Created on: Feb 18, 2019
 *      Author: check
 */

#include "PlayerSpotify.h"

ClPlayerSpotify::ClPlayerSpotify(const StSpotifyConfig oConfig):
ClPlayerBase(&m_oConfig),
m_oSpotifyAuth(uri("http://localhost:8080/spotify/auth")),
m_oLogger(ClLogger("Spotify")),
m_oConfig(oConfig)
{
	m_oSpotifyAuth.open().wait();
	m_oSpotifyAuth.support(methods::GET,  [this](http_request request){ this->handleSpotifyAuth(request); });
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

void ClPlayerSpotify::handleSpotifyAuth(http_request oRequest)
{
	std::cout << oRequest.request_uri().to_string().c_str() << std::endl;
	//send OK response
	http_response oResponse(status_codes::OK);
	oResponse.headers().add(U("Content-Type"), U("text/html"));
	oResponse.set_body("<html><head></head><body>Success!</body></html>");
	oRequest.reply(oResponse);
}
