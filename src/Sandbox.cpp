//============================================================================
// Name        : Sandbox.cpp
// Author      : check
// Version     :
// Copyright   : (c) check
// Description : Main Application
//============================================================================

#include <iostream>
#include <thread>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "PlayerPlugins/PlayerBase.h"
#include "ReaderPlugins/ReaderFile.h"

#include "AudioDatabase.h"
#include "Manager.h"
#include "Webserver.h"

auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("SandBox", "SandBox.log");

int main() {
	//initialize components
	auto spWebserver = std::make_shared<ClWebserver>();
	auto spAudioDb = std::make_shared<ClAudioDatabase>();
	auto spPlayer = std::make_shared<ClPlayerBase>();
	auto spReader = std::make_shared<ClReaderFile>();
	auto spManager = std::make_unique<ClManager>(spPlayer, spReader, spAudioDb);
	//start threads
	std::thread oReaderThread = std::thread(&ClReaderFile::start, spReader.get());
	std::thread oManagerThread = std::thread(&ClManager::start, spManager.get());
	//wait for threads to finish
	oManagerThread.join();
	oReaderThread.join();
	return 0;
}
