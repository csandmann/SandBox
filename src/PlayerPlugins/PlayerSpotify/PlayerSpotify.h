/*
 * PlayerSpotify.h
 *
 *  Created on: Feb 18, 2019
 *      Author: check
 */

#ifndef SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_
#define SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_

#include "../PlayerBase.h"
#include "../../Logging/Logger.h"
#include "SpotifyTokens.h"
#include "SpotifyMessage.h"
#include "SpotifyAuthorizationModule.h"
#include <boost/filesystem.hpp>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/uri_builder.h>
#include <boost/format.hpp>


using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
namespace fs = boost::filesystem;


struct StSpotifyConfig : StPlayerConfig
{
	std::string sClientId;
	std::string sClientSecret;
	std::string sHostname;
	unsigned int nPort;
	fs::path oCacheDir;
	std::string sDevice;
};
StSpotifyAuthorizationConfig authConfigFromSpotifyConfig(const StSpotifyConfig &stSpotifyConfig);

class ClPlayerSpotify : public ClPlayerBase
{
public:
	ClPlayerSpotify(const StSpotifyConfig oConfig);
	~ClPlayerSpotify() override;
	const std::string getIdentifier() const override;
	const bool restEndpointActive() const override;
	void execute(const std::vector<unsigned char> &vcMessage) override;
	void stop() override;
	void pause() override;
	void increaseVolume() override;
	void decreaseVolume() override;
    void resume() override;
    std::vector<unsigned char> getMessageToWrite() override;
private:
	//main functionality
	ClLogger m_oLogger;
	const StSpotifyConfig m_oConfig;
	//listeners
	http_listener m_oSpotifyMainSite;
	http_listener m_oSpotifyFormReceiver;
	//autho module
	ClSpotifyAuthorizationModule m_oAuthModule;
    //message to write
    std::vector<unsigned char> m_vcMessageToWrite;
	//
	SpotifyMessage::StSpotifyDevice m_stActiveDevice;
	//play functionality
	void playTrack(const std::string &sMessage);
	//http callbacks
	void cbkSpotifyMainSite(http_request oRequest);
	void cbkSpotifyFormReceiver(http_request oRequest);
	//helper functions
	const utility::string_t buildRedirectUri();
	bool spotifyResponseOk(const std::string &sFunction, const http_response &oResponse);
	boost::format readHtmlTemplateFromFile(const std::string &sFilePath);
	std::string updateActiveDevice();
};



#endif /* SRC_PLAYERPLUGINS_PLAYERSPOTIFY_H_ */
