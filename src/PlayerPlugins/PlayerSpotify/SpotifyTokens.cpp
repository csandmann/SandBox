/*
 * SpotifyTokens.cpp
 *
 *  Created on: Mar 4, 2019
 *      Author: check
 */
#include "SpotifyTokens.h"
#include <fstream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace SpotifyTokens
{

	StTokens::StTokens(){}

	StTokens::StTokens(const StTokens &other) :
	bIsInitialized(other.bIsInitialized),
	sAccessToken(other.sAccessToken),
	sRefreshToken(other.sRefreshToken)
	{}

	StTokens& StTokens::operator=(const StTokens &other)
	{
		this->bIsInitialized = other.bIsInitialized;
		this->sAccessToken = other.sAccessToken;
		this->sRefreshToken = other.sRefreshToken;
		return *this;
	}


	bool dumpTokens(const StTokens &stTokens, const std::string &sOutputFile)
	{
		try
		{
			const auto oBaseDir = fs::path(sOutputFile).parent_path();
			if (!fs::exists(oBaseDir)) {
				fs::create_directories(oBaseDir);
			}
			FILE* pfOut = fopen(sOutputFile.c_str(), "w");
			fprintf(pfOut, "%s\n%s", stTokens.sAccessToken.c_str(), stTokens.sRefreshToken.c_str());
			fclose(pfOut);
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	StTokens readTokens(const std::string &sInputFile)
	{
		StTokens stTokens;
		try
		{
			std::ifstream ifsIn(sInputFile.c_str());
			std::getline(ifsIn, stTokens.sAccessToken);
			std::getline(ifsIn, stTokens.sRefreshToken);
			ifsIn.close();
			stTokens.bIsInitialized = true;
		}
		catch (...)
		{
			stTokens.bIsInitialized = false;
		}
		return stTokens;
	}

}
