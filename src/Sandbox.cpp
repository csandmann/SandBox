//============================================================================
// Name        : Sandbox.cpp
// Author      : check
// Version     :
// Copyright   : (c) check
// Description : Main Application
//============================================================================

#include <iostream>
#include "AudioDatabase.h"

int main() {
	ClAudioDatabase oAudioDb;
	StAudioItem stItem = oAudioDb.audioItemFromKey(1234);
	return 0;
}
