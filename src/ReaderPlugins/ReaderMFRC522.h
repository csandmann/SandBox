/*
 * ReaderMFRC522.h
 *
 *  Created on: Feb 10, 2019
 *      Author: check
 */

#ifndef INCLUDE_READERMFRC522_H_
#define INCLUDE_READERMFRC522_H_

#include "ReaderBase.h"
#include "../Logging/Logger.h"
#include "../../3rdParty/RPi-RFID/MFRC522.h"

class ClReaderMFRC522 : public ClReaderBase
{
public:
	ClReaderMFRC522(const StReaderConfig oReaderConfig);
	~ClReaderMFRC522();
private:
	MFRC522 m_oReader;
	MFRC522::MIFARE_Key m_oKey;
	
	const std::vector<unsigned char> read() override;
	bool write(const std::vector<unsigned char> &stMsg) override;
	int getTrailerBlock(const int nSector);
	const StReaderConfig m_oReaderConfig;
	ClLogger m_oLogger;
};


#endif /* INCLUDE_READERMFRC522_H_ */
