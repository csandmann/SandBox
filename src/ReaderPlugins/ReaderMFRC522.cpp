/*
 * ReaderFile.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: check
 */
#include "ReaderMFRC522.h"
#include <string>
#include <iostream>

#define SECTORS 16
#define BLOCK_SIZE 16
#define BLOCKS_PER_SECTOR 4

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
    	if (!m_oReader.PICC_IsNewCardPresent() || !m_oReader.PICC_ReadCardSerial()) {
		return vcData;
	}
	m_oLogger.info("Read: New card detected");
	//init readout
	int nCurrentSector = 0;
	int nCurrentBlock = 1;	
	MFRC522::StatusCode nStatus;
	byte acBuffer[18];
	byte nSize = sizeof(acBuffer);
	//authenticate
	nStatus = (MFRC522::StatusCode) m_oReader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, getTrailerBlock(nCurrentSector), &m_oKey, &(m_oReader.uid));
	if (nStatus != MFRC522::STATUS_OK) {
		m_oLogger.error("read: PCD_Authenticate failed for sector " + std::to_string(nCurrentSector) + ": " + std::string(m_oReader.GetStatusCodeName(nStatus)));
        return vcData;
	} 
	//read
	nStatus = (MFRC522::StatusCode) m_oReader.MIFARE_Read(nCurrentBlock, acBuffer, &nSize);
    if (nStatus != MFRC522::STATUS_OK) {
	m_oLogger.error("read: MIFARE_Read failed for sector " + std::to_string(nCurrentSector) + " and block " + std::to_string(nCurrentBlock) + ": " + std::string(m_oReader.GetStatusCodeName(nStatus)));
		return vcData;
	}
	//check if card is valid
	if (std::memcmp(&acBuffer[0], "SBX", 3))
	{
		m_oLogger.info("read: Card Invalid (header does not start with 'SBX')");
		return vcData;
	}
	//get total length of data to read
	int nDataSize = *(reinterpret_cast<int*>(&acBuffer[3]));
	//copy first chunk and proceed
	vcData.resize(nDataSize);
	int nToRead = std::min(nDataSize, (int)(BLOCK_SIZE-sizeof(int)-3));
	std::memcpy(vcData.data(), &acBuffer[sizeof(int)+3], nToRead);
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
			if (nStatus != MFRC522::STATUS_OK) {
				m_oLogger.error("read: PCD_Authenticate failed for sector " + std::to_string(nCurrentSector) + ": " + std::string(m_oReader.GetStatusCodeName(nStatus)));
				vcData.resize(0);
				return vcData;
			}
		}
		for (;nCurrentBlock<(BLOCKS_PER_SECTOR-1); nCurrentBlock++)
		{
			nStatus = (MFRC522::StatusCode) m_oReader.MIFARE_Read(BLOCKS_PER_SECTOR*nCurrentSector + nCurrentBlock, acBuffer, &nSize);
			if (nStatus != MFRC522::STATUS_OK) {
				m_oLogger.error("read: MIFARE_Read failed for sector(s) " + std::to_string(nCurrentSector) + " and block " + std::to_string(nCurrentBlock) + ": " + std::string(m_oReader.GetStatusCodeName(nStatus)));
				vcData.resize(0);
				return vcData;
			}
			//copy to vector
			nToRead = std::min(nDataSize - nProcessedData, BLOCK_SIZE);
			std::memcpy(&vcData.at(nProcessedData), &acBuffer[0], nToRead);
			nProcessedData += nToRead;
			if (nProcessedData >= nDataSize)
			{
				bIsFinished = true;
				break;
			}
		}
		nCurrentBlock = 0;
	}
	m_oLogger.info("Read: Success, obtained" + std::to_string(vcData.size()) + " bytes");
	m_oLogger.debug("Read: Card data:");
	for (int i = 0; i<vcData.size(); i++)
	{
		m_oLogger.debug(std::to_string(i) + ": " + std::to_string(vcData[i]));
	}
	return vcData;
}


int ClReaderMFRC522::getTrailerBlock(const int nSector) {
	return (nSector+1) * 4 - 1;
}

bool ClReaderMFRC522::write(const std::vector<unsigned char> &vcData)
{
	m_oLogger.info("write: Starting to write " + std::to_string(vcData.size()) + " bytes");
	m_oLogger.debug("write: data");
	for (int i = 0; i<vcData.size(); i++)
	{
		m_oLogger.debug(std::to_string(i) + ": " + std::to_string(vcData[i]));
	}
	//setup vcDataWithHeader
	int nDataSizeWithoutHeader = vcData.size();
	std::vector<unsigned char> vcDataWithHeader(3+sizeof(int)+nDataSizeWithoutHeader);
	unsigned char* pcCurrentPos = vcDataWithHeader.data();
	//prepend first bytes
	const char* pcIdentifier = "SBX";
	std::memcpy(pcCurrentPos, reinterpret_cast<const unsigned char*>(pcIdentifier), 3);
	pcCurrentPos += 3;
	//prepend datasize
	std::memcpy(pcCurrentPos, reinterpret_cast<unsigned char*>(&nDataSizeWithoutHeader), sizeof(int));
	pcCurrentPos += sizeof(int);
	//copy data
	std::memcpy(pcCurrentPos, vcData.data(), vcData.size());
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
			break;
		}
		nStatus = (MFRC522::StatusCode) m_oReader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, getTrailerBlock(nCurrentSector), &m_oKey, &(m_oReader.uid));
		if (nStatus != MFRC522::STATUS_OK) {
			m_oLogger.error("write: PCD_Authenticate failed for sector " + std::to_string(nCurrentSector) + ": " + std::string(m_oReader.GetStatusCodeName(nStatus)));
			return false;
		}
		for (;nCurrentBlock < (BLOCKS_PER_SECTOR-1); nCurrentBlock++)
		{
			int nTmp = vcDataWithHeader.size() - nWrittenData;
			int nToWrite = std::min(nTmp, BLOCK_SIZE);
			std::vector<unsigned char> vcToWrite(BLOCK_SIZE);
			std::memcpy(vcToWrite.data(), &vcDataWithHeader[nWrittenData], nToWrite);
			nStatus = (MFRC522::StatusCode) m_oReader.MIFARE_Write(nCurrentSector*BLOCKS_PER_SECTOR + nCurrentBlock, vcToWrite.data(), BLOCK_SIZE);
			if (nStatus != MFRC522::STATUS_OK) 
			{
				m_oLogger.error("write: MIFARE_Write failed for sector " + std::to_string(nCurrentSector) + " and block " + std::to_string(nCurrentBlock) + ": " + std::string(m_oReader.GetStatusCodeName(nStatus)));
				return false;
			}
			nWrittenData += BLOCK_SIZE;
			if (nWrittenData >= vcDataWithHeader.size())
			{
				bIsFinished = true;
				break;
			}
		}
		nCurrentBlock = 0;
	}
	m_oLogger.debug("write: success");
	return true;
}
