/*
 * SpotifyTokens.h
 *
 *  Created on: Mar 4, 2019
 *      Author: check
 */

#ifndef SRC_PLAYERPLUGINS_PLAYERSPOTIFY_SPOTIFYTOKENS_H_
#define SRC_PLAYERPLUGINS_PLAYERSPOTIFY_SPOTIFYTOKENS_H_

#include <string>

namespace SpotifyTokens
{
	struct StTokens
	{
		bool bIsInitialized = false;
		std::string sAccessToken;
		std::string sRefreshToken;

		StTokens(const StTokens &other);
		StTokens();
		StTokens& operator=(const StTokens &other);
	};

	bool dumpTokens(const StTokens &stTokens, const std::string &sOutputFile);
	StTokens readTokens(const std::string &sInputFile);
}



#endif /* SRC_PLAYERPLUGINS_PLAYERSPOTIFY_SPOTIFYTOKENS_H_ */
