//============================================================================
// Name        : Sandbox.cpp
// Author      : check
// Version     :
// Copyright   : (c) check
// Description : Main Application
//============================================================================

#include <iostream>
#include <thread>
//plugins
#include "PlayerPlugins/PlayerSpotify.h"
#include "ReaderPlugins/ReaderFile.h"
//main stuff
#include "AudioDatabase.h"
#include "Manager.h"
#include "Webserver.h"
#include "Configuration.h"

int main() {
	//initialize components
	ClConfiguration oConfig;
	ClWebserver oWebserver(oConfig);
	ClAudioDatabase oAudioDb(oConfig);
	ClPlayerSpotify oSpotifyPlayer(oConfig);
	ClReaderFile oReader(oConfig);
	//create manager
	ClManager oManager(&oReader, &oAudioDb);
	oManager.registerPlayer(&oSpotifyPlayer);
	//start threads
	std::thread oReaderThread = std::thread(&ClReaderFile::start, &oReader);
	std::thread oManagerThread = std::thread(&ClManager::start, &oManager);
	//wait for threads to finish
	oManagerThread.join();
	oReaderThread.join();
	return 0;
}
