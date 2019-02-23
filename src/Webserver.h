/*
 * Webserver.h
 *
 *  Created on: Feb 14, 2019
 *      Author: check
 */

#ifndef INCLUDE_WEBSERVER_H_
#define INCLUDE_WEBSERVER_H_
#include <thread>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

#include "./Logging/Logger.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

struct StWebserverConfig
{
	std::string sHostname;
	unsigned int nPort;
};

class ClWebserver
{
public:
	ClWebserver(const StWebserverConfig stConfig);
	~ClWebserver();
private:
	http_listener m_oMainWebsite;
	void handleMainWebsite(http_request oRequest);
	ClLogger m_oLogger;
	const StWebserverConfig m_oConfig;
};

#endif /* INCLUDE_WEBSERVER_H_ */
