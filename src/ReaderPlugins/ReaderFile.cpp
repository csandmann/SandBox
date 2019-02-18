/*
 * ReaderFile.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: check
 */
#include "ReaderFile.h"
#include <fstream>
#include <string>

ClReaderFile::ClReaderFile():
m_oRFIDPath("/home/check/test/rfid")
{}

ClReaderFile::~ClReaderFile() {}

const StReaderMessage ClReaderFile::read()
{
	StReaderMessage stMsg;
	if (fs::exists(m_oRFIDPath))
	{
		std::string sNumber;
		std::ifstream fin(m_oRFIDPath.string().c_str());
		std::getline(fin, sNumber);
		fin.close();
		int nKey = std::stoi(sNumber);
		stMsg.nKey = nKey;
		stMsg.eStatus = EReaderStatus::DETECTED;
	}
	else
	{
		stMsg.eStatus = EReaderStatus::EMPTY;
	}
	return stMsg;
}
