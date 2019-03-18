#include "SpotifyMessage.h"

namespace SpotifyMessage{

    StMessage deserialize(const std::vector<unsigned char> &vcData)
    {
        StMessage stRet;
        //get command
        const ECommand *peCommand = reinterpret_cast<const ECommand*>(vcData.data());
        stRet.eCommand = *peCommand;
        //get arguments
        int iStart = sizeof(ECommand);
        stRet.sArguments = std::string(reinterpret_cast<const char*>(&vcData[iStart]));
        return stRet;
    }

    std::vector<unsigned char> serialize(const StMessage stMsg)
    {
        std::vector<unsigned char> vcOut;
        int nCmd = sizeof(ECommand);
        int nArgs = stMsg.sArguments.size() + 1;
        vcOut.resize(nCmd + nArgs);
        unsigned char* pcDest = vcOut.data();
        std::memcpy(pcDest, &stMsg.eCommand, nCmd);
        pcDest += sizeof(ECommand);
        std::memcpy(pcDest, stMsg.sArguments.c_str(), nArgs);
        return vcOut;
    }
}