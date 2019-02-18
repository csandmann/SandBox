//============================================================================
// Name        : Sandbox.cpp
// Author      : check
// Version     :
// Copyright   : (c) check
// Description : Main Application
//============================================================================

#include <iostream>
#include <thread>
//logging
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
//plugins
#include "PlayerPlugins/PlayerSpotify.h"
#include "ReaderPlugins/ReaderFile.h"
//main stuff
#include "AudioDatabase.h"
#include "Manager.h"
#include "Webserver.h"

auto oLogger = spdlog::basic_logger_mt<spdlog::async_factory>("SandBox", "SandBox.log");

int main() {
	//initialize components
	auto oWebserver = ClWebserver();
	auto oAudioDb = ClAudioDatabase();
	auto oPlayer = ClPlayerBase();
	auto oReader = ClReaderFile();
	auto oManager = ClManager(oPlayer, oReader, oAudioDb);
	//start threads
	std::thread oReaderThread = std::thread(&ClReaderFile::start, &oReader);
	std::thread oManagerThread = std::thread(&ClManager::start, &oManager);
	//wait for threads to finish
	oManagerThread.join();
	oReaderThread.join();
	return 0;
}
