#include "Webserver.h"
/*
 * Webserver.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: check
 */

ClWebserver::ClWebserver(const StWebserverConfig oConfig):
m_oMainWebsite(uri("http://localhost:8080")),
m_oLogger(ClLogger{"Webserver"}),
m_oConfig(oConfig)
{
	m_oMainWebsite.open().wait();
	m_oMainWebsite.support(methods::GET,  [this](http_request request){ this->handleMainWebsite(request); });
}


ClWebserver::~ClWebserver() {
}

void ClWebserver::handleMainWebsite(http_request oRequest)
{
	http_response oResponse(status_codes::OK);
	oResponse.headers().add(U("Content-Type"), U("text/html"));
	oResponse.set_body("<html><head></head><body>Hier gibt es nichts zu sehen, bitte gehen sie weiter.</body></html>");
	oRequest.reply(oResponse);
}

