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

#include "Configuration.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;


class ClWebserver
{
public:
	ClWebserver(const ClConfiguration &oConfig);
	~ClWebserver();
private:
	http_listener m_oMainWebsite;
	void handleMainWebsite(http_request oRequest);
	const ClConfiguration &m_oConfiguration;
};

#endif /* INCLUDE_WEBSERVER_H_ */
