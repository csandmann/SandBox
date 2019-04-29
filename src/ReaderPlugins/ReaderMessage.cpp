#include "ReaderMessage.h"
#include <cstring>

namespace ReaderMessage
{		
	bool operator ==(const StCardData& oThis, const StCardData& oOther)
	{
		return oThis.vcPlayerMessage == oOther.vcPlayerMessage && oThis.sPlayerIdentifier==oOther.sPlayerIdentifier && oThis.nVersionNumber == oOther.nVersionNumber;
	}
	
	bool operator !=(const StCardData& oThis, const StCardData& oOther)
	{
		return !(oThis==oOther);
	}
		
	void resetMessage(StMessage &stMsg)
	{
			stMsg.eStatus = EStatus::EMPTY;
			StCardData stClearedData;
			stMsg.stCardData = stClearedData;
	}
	
	void resetCardData(StCardData &stCardData)
	{
		stCardData.sPlayerIdentifier = "";
		stCardData.vcPlayerMessage.resize(0);
	}
	
	std::vector<unsigned char> serializeCardData(const StCardData &stData)
	{
		int nVersionNumber = sizeof(unsigned char);
		int nIdentifier = strlen(stData.sPlayerIdentifier.c_str());
		int nData = stData.vcPlayerMessage.size();
		std::vector<unsigned char> vcDataToWrite(nVersionNumber + nIdentifier + nData);
		unsigned char *pcHead = vcDataToWrite.data();
		std::memcpy(pcHead, reinterpret_cast<unsigned char*>(&nVersionNumber), sizeof(int));
		pcHead += sizeof(int);
		std::memcpy(pcHead, stData.sPlayerIdentifier.c_str(), nIdentifier);
		pcHead += nIdentifier;
		std::memcpy(pcHead, stData.vcPlayerMessage.data(), nData);
		return vcDataToWrite;
	}
	
	StMessage deserializeCardData(const std::vector<unsigned char> vcData)
	{
		StMessage stMsg;
		if (vcData.size() > 0)
		{
			try
			{
				int nVersionNumber = sizeof(unsigned char);
				int nIdentifier = strlen(reinterpret_cast<const char*>(&vcData.at(nVersionNumber)));
				int nData = vcData.size() - nIdentifier - nVersionNumber;
				//set version number
				stMsg.stCardData.nVersionNumber = reinterpret_cast<unsigned char>(vcData.at(0));
				//set identifier
				stMsg.stCardData.sPlayerIdentifier = std::string(reinterpret_cast<const char*>(vcData.at(nVersionNumber)));
				//set message
				stMsg.stCardData.vcPlayerMessage.resize(nData);
				std::memcpy(stMsg.stCardData.vcPlayerMessage.data(), &vcData[nVersionNumber + nIdentifier], nData);
				stMsg.eStatus = EStatus::DETECTED;
				return stMsg;
			}
			catch (...)
			{
				stMsg.eStatus = EStatus::EMPTY;
				stMsg.stCardData.sPlayerIdentifier = "";
				stMsg.stCardData.vcPlayerMessage.resize(0);
			}
		}
		return stMsg;
	}
}