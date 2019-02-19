/*
 * PlayerSpotify.cpp
 *
 *  Created on: Feb 18, 2019
 *      Author: check
 */

#include "PlayerSpotify.h"

ClPlayerSpotify::ClPlayerSpotify():
m_oSpotifyAuth(uri("http://localhost:8080/spotify/auth"))
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
	std::cout << "Spotify: Playing" << pcMessage <<  std::endl;
}

void ClPlayerSpotify::stop()
{
	std::cout << "Spotify stopped" <<  std::endl;
}

void ClPlayerSpotify::pause()
{
	std::cout << "Spotify paused" <<  std::endl;
}

void ClPlayerSpotify::increaseVolume() {
}

void ClPlayerSpotify::decreaseVolume() {
}

void ClPlayerSpotify::handleSpotifyAuth(http_request oRequest)
{
	std::cout << oRequest.request_uri().to_string().c_str() << std::endl;
	http_response oResponse(status_codes::OK);
	oResponse.headers().add(U("Content-Type"), U("text/html"));
	oResponse.set_body("<html><head></head><body>Success!</body></html>");
	oRequest.reply(oResponse);
}
