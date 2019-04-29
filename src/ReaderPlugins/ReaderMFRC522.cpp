/*
 * ReaderFile.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: check
 */
#include "ReaderMFRC522.h"
#include <string>

#define SECTORS 16
#define BLOCK_SIZE 16
#define BLOCKS_PER_SECTOR = 3

ClReaderMFRC522::ClReaderMFRC522(const StReaderConfig oReaderConfig):
ClReaderBase(&m_oReaderConfig),
m_oReaderConfig(oReaderConfig),
m_oLogger(ClLogger("ReaderMFRC522"))
{
	m_oReader.PCD_Init();
	//setup default key
	for (byte i = 0; i < 6; i++) {
	  m_oKey.keyByte[i] = 0xFF;
	}
}

ClReaderMFRC522::~ClReaderMFRC522() {}

const std::vector<unsigned char> ClReaderMFRC522::read()
{
	std::vector<unsigned char> vcData;
	//check if card is present and read uuid
	if ( ! m_oReader.PICC_IsNewCardPresent()) {
		return vcData;
	}
    // Select one of the cards
    if ( ! m_oReader.PICC_ReadCardSerial()) {
		return vcData;
	}
	//init readout
	int nCurrentSector = 0;
	int nCurrentBlock = 1;	
	MFRC522::StatusCode nStatus;
	byte acBuffer[BLOCK_SIZE];
	byte nSize = sizeof(buffer);
	//authenticate
	nStatus = (MFRC522::StatusCode) m_oReader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, getTrailerBlock(nCurrentSector), &m_oKey, &(m_oReader.uid));
	if (status != MFRC522::STATUS_OK) {
		m_oLogger.error("read: PCD_Authenticate failed for sector " + std::to_string(nCurrentSector) + ": " + std::string(m_oReader.GetStatusCodeName(status)));
        return vcData;
	} 
	//read
	status = (MFRC522::StatusCode) m_oReader.MIFARE_Read(nCurrentBlock, acBuffer, &nSize);
    if (status != MFRC522::STATUS_OK) {
	m_oLogger.error("read: MIFARE_Read failed for sector " + std::to_string(nCurrentSector) + " and block " + std::to_string(nCurrentBlock) + ": " + std::string(m_oReader.GetStatusCodeName(status)));
		return vcData;
	}
	//get total length of data to read
	int nDataSize = *(static_cast<int*>(&acBuffer[0]));
	if (nDataSize <= 0)
	{
		return vcData;
	}
	//copy first chunk and proceed
	vcData.resize(nDataSize);
	int nToRead = std::min(nDataSize, BLOCK_SIZE-sizeof(int));
	std::memcpy(vcData.data(), &acBuffer[sizeof(int)], nToRead);
	int nProcessedData = nToRead;
	nCurrentBlock += 1;
	//read rest
	bool bIsFinished = false;
	for (;nCurrentSector<SECTORS; nCurrentSector++)
	{
		if (bIsFinished) {
			break;
		}
		if (nCurrentSector > 0) //first sector is already authenticated
		{
			nStatus = (MFRC522::StatusCode) m_oReader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, getTrailerBlock(nCurrentSector), &m_oKey, &(m_oReader.uid));
			if (status != MFRC522::STATUS_OK) {
				m_oLogger.error("read: PCD_Authenticate failed for sector " + std::to_string(nCurrentSector) + ": " + std::string(m_oReader.GetStatusCodeName(status)));
				vcData.resize(0);
				return vcData;
			}
		}
		for (;nCurrentBlock<BLOCKS_PER_SECTOR; nCurrentBlock++)
		{
			status = (MFRC522::StatusCode) m_oReader.MIFARE_Read(BLOCKS_PER_SECTOR*nCurrentSector + nCurrentBlock, acBuffer, &nSize);
			if (status != MFRC522::STATUS_OK) {
				m_oLogger.error("read: MIFARE_Read failed for sector " + std::to_string(nCurrentSector) + " and block " + std::to_string(nCurrentBlock) + ": " + std::string(m_oReader.GetStatusCodeName(status)));
				vcData.resize(0);
				return vcData;
			}
			//copy to vector
			nToRead = std::min(nDataSize - nProcessedData, BLOCK_SIZE);
			std::memcpy(&vcData.at(nProcessedData), &acBuffer[0], nToRead);
			nProcessedData += nToRead;
			if (nProcessedData.size() >= nDataSize)
			{
				bIsFinished = true;
				break;
			}
		}
		nCurrentBlock = 0;
	}
	return vcData;
}


int ClReaderMFRC522::getTrailerBlock(const int nSector) {
	return nSector * 4 - 1;
}

bool ClReaderMFRC522::write(const std::vector<unsigned char> &vcData)
{
	//check if card is present and read uuid
	if ( ! m_oReader.PICC_IsNewCardPresent()) {
		return false;
	}
    // Select one of the cards
    if ( ! m_oReader.PICC_ReadCardSerial()) {
		return false;
	}
	//prepend datasize
	int nDataSize = vcData.size() + sizeof(int);
	unsigned int* pnDataSize = reinterpret_cast<unsigned char>(&nDataSize);
	for (int i = 0; i< sizeof(int); ++i)
	{
		vcData.insert(vcData.begin() + i, pnDataSize[i]);
	}
	//init writeout
	int nWrittenData = 0;
	int nCurrentSector = 0;
	int nCurrentBlock = 1;	
	MFRC522::StatusCode nStatus;
	//write
	bool bIsFinished = false;
	for (;nCurrentSector < SECTORS; nCurrentSector++)
	{
		if (bIsFinished) {
			break
		}
		nStatus = (MFRC522::StatusCode) m_oReader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, getTrailerBlock(nCurrentSector), &m_oKey, &(m_oReader.uid));
		if (status != MFRC522::STATUS_OK) {
			m_oLogger.error("write: PCD_Authenticate failed for sector " + std::to_string(nCurrentSector) + ": " + std::string(m_oReader.GetStatusCodeName(status)));
			return false;
		}
		for (;nCurrentBlock < BLOCKS_PER_SECTOR; nCurrentBlock++)
		{
			int nToWrite = std::min(vcData.size() - nDataWritten, BLOCK_SIZE);
			status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(nCurrentSector*BLOCKS_PER_SECTOR + nCurrentBlock, vcData[nDataWritten], nToWrite);
			if (status != MFRC522::STATUS_OK) 
			{
				m_oLogger.error("read: MIFARE_Write failed for sector " + std::to_string(nCurrentSector) + " and block " + std::to_string(nCurrentBlock) + ": " + std::string(m_oReader.GetStatusCodeName(status)));
				return false;
			}
			nWrittenData += BLOCK_SIZE;
			if (nWrittenData >= vcData.size())
			{
				bIsFinished = true;
				break;
			}
		}
		nCurrentBlock = 0;
	}
	return true;
}
