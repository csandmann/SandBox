#include "Webserver.h"


#define S2U(x) utility::conversions::to_string_t(x)
#define U2S(x) utility::conversions::to_utf8string(x)

/*
 * Webserver.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: check
 */

ClWebserver::ClWebserver(const StWebserverConfig oConfig):
m_oMainWebsite(uri(U("http://localhost:8080"))),
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
	auto oWebsite = readTemplateFromFile(m_oConfig.sResourceDir + "/Logger.html");
	auto oLog = readTemplateFromFile(m_oConfig.sLogFilePath);
	oWebsite % oLog.str();
	oResponse.set_body(S2U(oWebsite.str()));
	oRequest.reply(oResponse);
}


boost::format ClWebserver::readTemplateFromFile(const std::string &sFilePath)
{
	std::vector<char> vcOut;
	try
	{
		std::ifstream fsIn;
		fsIn.open(sFilePath.c_str(), std::ios::in);
		fsIn.seekg(0, std::ios::end);
		auto nSize = static_cast<int>(fsIn.tellg());
		fsIn.seekg(0, std::ios::beg);
		//read
		vcOut.resize(nSize);
		fsIn.read(vcOut.data(), nSize);
		return boost::format(vcOut.data());	
	}
	catch (...)
	{
		m_oLogger.error("readTemplateFromFile: Could not read resource " + sFilePath);
		return boost::format();
	}
}

