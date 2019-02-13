#include "Player.h"

ClPlayer::ClPlayer() {
}

ClPlayer::~ClPlayer() {
}

void ClPlayer::play(const StAudioItem& stMsg) {
	printf("Starting playback of %s (source: %i)\n", stMsg.sAudioInfo.c_str(), (int) stMsg.eSource);
}

void ClPlayer::stop() {
	printf("Playback stopped.\n");
}

void ClPlayer::pause() {
}

void ClPlayer::playSpotify(std::string sMessage) {
}
