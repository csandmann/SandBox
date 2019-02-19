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
#include <spdlog/spdlog.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;


class ClWebserver
{
public:
	ClWebserver();
	~ClWebserver();
private:
	http_listener m_oMainWebsite;
	void handleMainWebsite(http_request oRequest);
	//std::shared_ptr<spdlog::logger> m_oLogger;
};

#endif /* INCLUDE_WEBSERVER_H_ */
