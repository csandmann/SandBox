/*
 * SpotifyTokens.h
 *
 *  Created on: Mar 4, 2019
 *      Author: check
 */

#ifndef SRC_PLAYERPLUGINS_PLAYERSPOTIFY_SPOTIFYMESSAGE_H_
#define SRC_PLAYERPLUGINS_PLAYERSPOTIFY_SPOTIFYMESSAGE_H_

#include <vector>
#include <string>
#include <cstring>


namespace SpotifyMessage
{
	struct StSpotifyDevice
	{
		std::string sId;
		std::string sName;
	};
    
    enum class ECommand
    {
        playTrack = 0,
		playAlbum = 1,
		playPlaylist = 2,
		changePlayer = 3
    };
    
    struct StMessage
    {
        ECommand eCommand;
        std::string sArguments;
        
        StMessage(){};
    };

    StMessage deserialize(const std::vector<unsigned char> &vcData);
    std::vector<unsigned char> serialize(const StMessage stMsg);
}




#endif /*SRC_PLAYERPLUGINS_PLAYERSPOTIFY_SPOTIFYMESSAGE_H_*/
