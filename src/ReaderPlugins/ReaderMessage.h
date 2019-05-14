/*
 * ReaderMessage.h
 *
 *  Created on: Mar 15, 2019
 *      Author: check
 */

#ifndef INCLUDE_READERMESSAGE_H_
#define INCLUDE_READERMESSAGE_H_

#include <string>
#include <vector>

namespace ReaderMessage
{
	
	enum class EStatus
	{
		DETECTED = 0,
		EMPTY = 1,
	};
	
	struct StCardData
	{
		unsigned char nVersionNumber;
		std::string sPlayerIdentifier;
		std::vector<unsigned char> vcPlayerMessage;		
	};
	bool operator ==(const StCardData& oThis, const StCardData& oOther);
	bool operator !=(const StCardData& oThis, const StCardData& oOther);
	std::vector<unsigned char> serializeCardData(const StCardData &stData);
	void resetCardData(StCardData &stCardData);

	
	struct StMessage
	{
		EStatus eStatus;
		StCardData stCardData;

		StMessage() :
		eStatus(EStatus::EMPTY)
		{}
		
		StMessage& operator=(const StMessage &oOther)
		{
			eStatus = oOther.eStatus;
			stCardData = oOther.stCardData;
			return *this;
		}
		
	};
	void resetMessage(StMessage &stMsg);
	StMessage deserializeCardData(const std::vector<unsigned char> vcData);	
}



#endif /*INCLUDE_READERMESSAGE_H_*/
