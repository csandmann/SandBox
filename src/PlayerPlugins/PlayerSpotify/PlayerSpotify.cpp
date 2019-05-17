/*
 * PlayerSpotify.cpp
 *
 *  Created on: Feb 18, 2019
 *      Author: check
 */

#include "PlayerSpotify.h"
#include <sstream>
#include <fstream>

#define S2U(x) utility::conversions::to_string_t(x)
#define U2S(x) utility::conversions::to_utf8string(x)

using namespace web::http::client;
using namespace web::json;

StSpotifyAuthorizationConfig authConfigFromSpotifyConfig(const StSpotifyConfig &stSpotifyConfig)
{
	StSpotifyAuthorizationConfig stAuthConfig;
	stAuthConfig.nPort = stSpotifyConfig.nPort;
	stAuthConfig.sClientId = stSpotifyConfig.sClientId;
	stAuthConfig.sClientSecret = stSpotifyConfig.sClientSecret;
	stAuthConfig.sHostname = stSpotifyConfig.sHostname;
	stAuthConfig.sTokenFilePath = (stSpotifyConfig.oCacheDir / fs::path("spotifyTokens.txt")).string();
	return stAuthConfig;
}

ClPlayerSpotify::ClPlayerSpotify(const StSpotifyConfig oConfig):
ClPlayerBase(&m_oConfig),
m_oLogger(ClLogger("Spotify")),
m_oConfig(oConfig),
m_oSpotifyMainSite(uri(U(str(boost::format("http://%1%:%2%/spotify") % oConfig.sHostname % oConfig.nPort)))),
m_oSpotifyFormReceiver(uri(U(str(boost::format("http://%1%:%2%/spotify/form_receiver") % oConfig.sHostname % oConfig.nPort)))),
m_oAuthModule(authConfigFromSpotifyConfig(oConfig))
{
	//start listeners for websites
	m_oSpotifyMainSite.open().wait();
	m_oSpotifyMainSite.support(methods::GET,  [this](http_request request){ this->cbkSpotifyMainSite(request); });
	m_oSpotifyFormReceiver.open().wait();
	m_oSpotifyFormReceiver.support(methods::POST,  [this](http_request request){ this->cbkSpotifyFormReceiver(request); });
	//refresh accesstokens if they exist
	if (m_oAuthModule.isInitialized())
	{
		updateActiveDevice();
	}
}

ClPlayerSpotify::~ClPlayerSpotify() {
}

const std::string ClPlayerSpotify::getIdentifier() const {
	return std::string("spotify");
}

const bool ClPlayerSpotify::restEndpointActive() const {
	return true;
}

void ClPlayerSpotify::playTrack(const std::string &sMessage)
{
	m_oLogger.info(std::string("playTrack: ") + sMessage);
	//build body
	auto oURIs = json::value::array();
	oURIs[0] = json::value::string(S2U(sMessage));
	auto oBody = json::value::object();
	oBody[U("uris")] = oURIs;
	std::string sBody = U2S(oBody.serialize());
	//make request
	executeSpotifyCommand("https://api.spotify.com/v1/me/player/play", methods::PUT, sBody);
}

void ClPlayerSpotify::playContext(const std::string &sMessage)
{
	m_oLogger.info(std::string("playContext: ") + sMessage);
	//build body
	auto oBody = json::value::object();
	oBody[U("context_uri")] = json::value(S2U(sMessage));
	utility::string_t sBody = U2S(oBody.serialize());
	//make request
	executeSpotifyCommand("https://api.spotify.com/v1/me/player/play", methods::PUT, sBody);
}

void ClPlayerSpotify::execute(const std::vector<unsigned char> &vcMessage)
{
	SpotifyMessage::StMessage stMsg = SpotifyMessage::deserialize(vcMessage);
	switch (stMsg.eCommand)
	{
		case SpotifyMessage::ECommand::playTrack: 
			playTrack(stMsg.sArguments);
			break;
		case SpotifyMessage::ECommand::playAlbum:
		{
			playContext(stMsg.sArguments);
			break;
		}
		case SpotifyMessage::ECommand::playPlaylist:
		{
			playContext(stMsg.sArguments);
			break;
		}
		case SpotifyMessage::ECommand::changePlayer:
		{
			m_stActiveDevice.sName = stMsg.sArguments;
			m_stActiveDevice.sId = "";
			updateActiveDevice();
			break;
		}
		default:
			m_oLogger.error("execute: Unknown command: " + std::to_string(static_cast<int>(stMsg.eCommand)) + " with arguments " + stMsg.sArguments);
	}
}

std::string ClPlayerSpotify::updateActiveDevice()
{
	m_oLogger.info("updateActiveDevice: Activating " + m_oConfig.sDevice);
	std::vector<SpotifyMessage::StSpotifyDevice> voDeviceList;
	//build request
	http_request oRequest(methods::GET);
	oRequest.headers().add(U("Content-Type"), U("application/json"));
	oRequest.headers().add(U("Authorization"), utility::string_t(U("Bearer ")) + S2U(m_oAuthModule.getAccessToken()));
	//make request
	http_client oClient(U("https://api.spotify.com/v1/me/player/devices"));
	pplx::task<std::vector<SpotifyMessage::StSpotifyDevice> > oTask = oClient.request(oRequest)
		.then([&](http_response response) -> pplx::task<json::value> {
	            if(response.status_code() == status_codes::OK){
	                return response.extract_json();
	            }
				else if(response.status_code() == status_codes::BadRequest){
					this->m_oLogger.warn("updateActiveDevice: Not authorized. Refreshing Access Token ");
					this->m_oAuthModule.refreshAccessToken();
					return pplx::task_from_result(json::value());
				} 
				else {
	            	this->m_oLogger.error(std::string("updateActiveDevice: Could not parse response ") + U2S(response.to_string()));
	            	return pplx::task_from_result(json::value());
	            };
		})
		.then([&](pplx::task<json::value> oPreviousTask)
		{	            
			std::vector<SpotifyMessage::StSpotifyDevice> voDeviceList;
			try{
					const json::value &oJson = oPreviousTask.get();
					const auto oRet = oJson.as_object();
					const auto oJsonDeviceList = oRet.at(U("devices"));
					for (unsigned int i = 0; i<oJsonDeviceList.size(); ++i)
					{
						const auto oDevice = oJsonDeviceList.at(i).as_object();
						SpotifyMessage::StSpotifyDevice stDevice{oDevice.at(U("id")).as_string().c_str(), oDevice.at(U("name")).as_string().c_str()};
						voDeviceList.push_back(stDevice);
					}
	            }
			catch(const http_exception &e){
	            	this->m_oLogger.error(std::string("updateActiveDevice: Could not unpack JSON response: ") + std::string(e.what()));
			}
			return voDeviceList;
		});
	//execute task and get response
	try {
		oTask.wait();
		voDeviceList = oTask.get();
	}
	catch (std::exception &e) {
		m_oLogger.error(std::string("getDeviceList: Could not perform request: ") + std::string(e.what()));
	}
	//get correct device
	std::string sAllDevices;
	bool bPlaybackDeviceFound = false;
	for (const auto &stDevice : voDeviceList)
	{
		if (m_oConfig.sDevice == stDevice.sName)
		{
			m_stActiveDevice = stDevice;
			bPlaybackDeviceFound = true;
		}
		sAllDevices += stDevice.sName;
		if (stDevice.sName != voDeviceList.back().sName)
		{
			sAllDevices += ", ";
		}
	}
	if (!bPlaybackDeviceFound) {
		m_oLogger.error("getDeviceList: " + m_oConfig.sDevice + " was not found in active devices. Is it turned on?");
	}
	return sAllDevices;
}

void ClPlayerSpotify::stop()
{
	m_oLogger.info(std::string("stopping"));
	executeSpotifyCommand("https://api.spotify.com/v1/me/player/pause", methods::PUT);
}

bool ClPlayerSpotify::spotifyResponseOk(const http_response &oResponse)
{
	if (oResponse.status_code() == status_codes::NoContent) 
	{
		return true;
	}
	else if (oResponse.status_code() == status_codes::Unauthorized)
	{
		m_oLogger.error("spotifyResponseOk: Unautorized");
		m_oAuthModule.refreshAccessToken();
		return false;
	}
	else if (oResponse.status_code() == status_codes::BadRequest) 
	{
		m_oLogger.error("spotifyResponseOk: Bad request");
		return false;
	}
	else if (oResponse.status_code() == status_codes::NotFound)
	{
		m_oLogger.error("spotifyResponseOk: Not found. Is the active device correct?");
		return false;
	}
	else {
		m_oLogger.error("spotifyResponseOk: Could not parse response " + U2S(oResponse.to_string()));
		return false;
	}
}

void ClPlayerSpotify::resume()
{
    m_oLogger.info("Resuming");
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


boost::format ClPlayerSpotify::readHtmlTemplateFromFile(const std::string &sFilePath)
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
		m_oLogger.error("readHtmlTemplateFromFile: Could not read resource " + sFilePath);
		return boost::format();
	}
}

void ClPlayerSpotify::cbkSpotifyMainSite(http_request oRequest)
{
	std::string sAllDevices = updateActiveDevice();
	//send Website response
	http_response oResponse(status_codes::OK);
	oResponse.headers().add(U("Content-Type"), U("text/html"));
	boost::format oWebsite = readHtmlTemplateFromFile(m_oConfig.sResourceDir +  "/Spotify.html");
	oWebsite % m_oConfig.sHostname % m_oConfig.nPort % sAllDevices % U2S(m_oAuthModule.getSpotifyAuthorizationUri());
	//std::stringstream ss;ss.str()
	//ss << "<html> <head></head><body><a href=\"" << U2S(buildSpotifyAuthorizationUri()) << "\"> Connect Account </a> </body> </html>";
	oResponse.set_body(S2U(oWebsite.str()));
	oRequest.reply(oResponse);
}
	
std::vector<unsigned char> ClPlayerSpotify::getMessageToWrite()
{
	if (m_vcMessageToWrite.size() == 0)
	{
		return m_vcMessageToWrite;
	}
	std::vector<unsigned char> vcMessage(std::move(m_vcMessageToWrite));
	m_vcMessageToWrite.resize(0);
	return vcMessage;
}

void ClPlayerSpotify::cbkSpotifyFormReceiver(http_request oRequest)
{
	m_oLogger.debug("cbkSpotifyFormReceiver: Incoming request");
	//get body and process request
	pplx::task<void> oTask = oRequest.extract_string().then([&](pplx::task<utility::string_t> oBody){
		//split into arguments
		auto oRequestArgs = uri::split_query(oBody.get());
		//get command
		SpotifyMessage::ECommand eCmd;
		if (oRequestArgs.find(U("type")) != oRequestArgs.end())
		{
			std::string sType = U2S(oRequestArgs.at(U("type")));
			if (sType == "track")
			{
				eCmd = SpotifyMessage::ECommand::playTrack;
			}
			else if (sType == "player")
			{
				eCmd = SpotifyMessage::ECommand::changePlayer;
			}
			else if (sType == "playlist")
			{
				eCmd = SpotifyMessage::ECommand::playPlaylist;
			}
			else if (sType == "album")
			{
				eCmd = SpotifyMessage::ECommand::playAlbum;
			}
			else
			{
				this->m_oLogger.warn("cbkSpotifyFormReceiver: Could not handle type " + sType);
				return;
			}
		}
		else {
			return;
		}
		//get message
		std::string sMessage;
		if (oRequestArgs.find(U("message")) != oRequestArgs.end())
		{
			sMessage = U2S(uri::decode(oRequestArgs.at(U("message"))));
		}
		else {
			return;
		}
		//build message to write
		//this->m_oLogger.debug("cbkSpotifyFormReceiver: Command/Message: " + std::to_string(eCmd) + " / " + sMessage);
		SpotifyMessage::StMessage stMsg;
		stMsg.eCommand = eCmd;
		stMsg.sArguments = sMessage;
		this->m_vcMessageToWrite = SpotifyMessage::serialize(stMsg);		
		});
	//execute task
	bool bSuccess = true; 
	try {
		oTask.wait();
	}
	catch(std::exception &e){
		m_oLogger.error(std::string("cbkSpotifyFormReceiver: Could not handle request request: ") + U2S(oRequest.to_string()));
		bSuccess = false;
	}	
	//send response
	http_response oResponse(status_codes::OK);
	oResponse.headers().add(U("Content-Type"), U("text/html"));
	if (!bSuccess)
	{
		oResponse.set_body("<html><head></head><body>Error (see log)!</body></html>");
	}
	else
	{
		oResponse.set_body("<html><head></head><body>Success!</body></html>");
	}
	oRequest.reply(oResponse);        
}

void ClPlayerSpotify::executeSpotifyCommand(const std::string &sUri, const method &eMethod, const std::string sBody/*=""*/, const bool bRepeatRequestIfFailed/*=true*/)
{
	//build request
	http_request oRequest(eMethod);
	oRequest.headers().add(U("Content-Type"), U("application/json"));
	oRequest.headers().add(U("Authorization"), utility::string_t(U("Bearer ")) + S2U(m_oAuthModule.getAccessToken()));
	oRequest.set_body(S2U(sBody));
	//make request
	http_client oClient(S2U(sUri) + U("?device_id=") + U(m_stActiveDevice.sId));
	pplx::task<bool> oTask = oClient.request(oRequest)
		.then([&](http_response response) {
		return this->spotifyResponseOk(response); 
		});
	//execute task
	bool bSuccess = false;
	try {
		oTask.wait();
		bSuccess = oTask.get();
	}
	catch (std::exception &e) {
		m_oLogger.error("executeSpotifyCommand: Could not perform request: " + std::string(e.what()));
	}
	if (!bSuccess && bRepeatRequestIfFailed) {
		m_oLogger.warn("executeSpotifyCommand: Repeating request");
			executeSpotifyCommand(sUri, eMethod, sBody, false);
	}
	
}
