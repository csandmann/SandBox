//============================================================================
// Name        : Sandbox.cpp
// Author      : Constantin Sandmann
// Version     :
// Copyright   : (c) check
// Description : Main Application
//============================================================================

//main stuff
#include "Configuration.h"
#include "Manager.h"
#include "Webserver.h"
#include "./Logging/BaseLogger.h"
//plugins
#include "PlayerPlugins/PlayerSpotify/PlayerSpotify.h"
#include "ReaderPlugins/ReaderFile.h"
//standard libs
#include <iostream>
#include <thread>


int main() {
	//initialize config and logging
	ClConfiguration oConfig;
	ClBaseLogger::init(oConfig.getLoggerConfig());
	//initialize components
	ClWebserver oWebserver(oConfig.getWebserverConfig());
	ClPlayerSpotify oPlayerSpotify(oConfig.getSpotifyConfig());
	ClReaderFile oReader(oConfig.getReaderConfig());
	//create manager
	ClManager oManager(&oReader);
	oManager.registerPlayer(&oPlayerSpotify);
	//start threads
	std::thread oReaderThread = std::thread(&ClReaderFile::start, &oReader);
	std::thread oManagerThread = std::thread(&ClManager::start, &oManager);
	//wait for threads to finish
	oManagerThread.join();
	oReaderThread.join();
	return 0;
}
