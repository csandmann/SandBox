//============================================================================
// Name        : Sandbox.cpp
// Author      : check
// Version     :
// Copyright   : (c) check
// Description : Main Application
//============================================================================

#include <iostream>
#include <thread>
#include "AudioDatabase.h"
#include "Manager.h"
#include "Player.h"
#include "ReaderFile.h"
#include "Webserver.h"

int main() {
	//initialize components
	auto spWebserver = std::make_shared<ClWebserver>();
	auto spAudioDb = std::make_shared<ClAudioDatabase>();
	auto spPlayer = std::make_shared<ClPlayer>();
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
