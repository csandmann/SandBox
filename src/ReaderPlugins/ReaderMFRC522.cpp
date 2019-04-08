/*
 * ReaderFile.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: check
 */
#include "ReaderMFRC522.h"
#include <fstream>
#include <string>

ClReaderMFRC522::ClReaderMFRC522(const StReaderConfig oReaderConfig):
ClReaderBase(&m_oReaderConfig),
m_oReaderConfig(oReaderConfig),
m_oLogger(ClLogger("ReaderMFRC522"))
{
  m_oReader.PCD_Init();
}

ClReaderMFRC522::~ClReaderMFRC522() {}

const std::vector<unsigned char> ClReaderMFRC522::read()
{
	std::vector<unsigned char> vcData;
	if(m_oReader.PICC_IsNewCardPresent() && m_oReader.PICC_ReadCardSerial())
	{
		// Print UID
		for(byte i = 0; i < mfrc.uid.size; ++i)
		{
			if(mfrc.uid.uidByte[i] < 0x10)
			{
				printf(" 0");
				printf("%X",mfrc.uid.uidByte[i]);
		  }
		  else
		  {
				printf(" ");
				printf("%X", mfrc.uid.uidByte[i]);
		  }
		}
		printf("\n");
	}
	return vcData;
}

bool ClReaderMFRC522::write(const std::vector<unsigned char> &vcData)
{
	return true;
}
