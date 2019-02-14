#include "Webserver.h"
/*
 * Webserver.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: check
 */

ClWebserver::ClWebserver():
m_oMainWebsite(uri("http://localhost:8080")),
m_oSpotifyAuth(uri("http://localhost:8080/auth"))
{
	m_oMainWebsite.open().wait();
	m_oMainWebsite.support(methods::GET,  [this](http_request request){ this->handleMainWebsite(request); });

	m_oSpotifyAuth.open().wait();
	m_oSpotifyAuth.support(methods::GET,  [this](http_request request){ this->handleSpotifyAuth(request); });

}


ClWebserver::~ClWebserver() {
}

void ClWebserver::handleSpotifyAuth(http_request oRequest)
{
	std::cout << oRequest.request_uri().to_string().c_str() << std::endl;
	http_response oResponse(status_codes::OK);
	oResponse.headers().add(U("Content-Type"), U("text/html"));
	oResponse.set_body("<html><head></head><body>Success!</body></html>");
	oRequest.reply(oResponse);
}

void ClWebserver::handleMainWebsite(http_request oRequest)
{
	http_response oResponse(status_codes::OK);
	oResponse.headers().add(U("Content-Type"), U("text/html"));
	oResponse.set_body("<html><head></head><body>Hier gibt es nichts zu sehen, bitte gehen sie weiter.</body></html>");
	oRequest.reply(oResponse);
}

