/*
 * ReaderFile.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: check
 */
#include "ReaderFile.h"
#include <fstream>
#include <string>

ClReaderFile::ClReaderFile(const StReaderConfig oReaderConfig):
ClReaderBase(&m_oReaderConfig),
m_oRFIDPath("test/rfid"),
m_oReaderConfig(oReaderConfig),
m_oLogger(ClLogger("ReaderFile"))
{}

ClReaderFile::~ClReaderFile() {}

const std::vector<unsigned char> ClReaderFile::read()
{
	std::vector<unsigned char> vcData;
	if (fs::exists(m_oRFIDPath))
	{
		std::ifstream fin(m_oRFIDPath.string().c_str(), std::ios::binary);
        //get data size
        fin.seekg(0, std::ios::end);
        int nSize = fin.tellg();
        fin.seekg(0, std::ios::beg);
        //read
		vcData.resize(nSize);
        fin.read(reinterpret_cast<char*>(vcData.data()), nSize);
		fin.close();
		//return
		return vcData;
	}
	return vcData;
}

void ClReaderFile::write(const std::vector<unsigned char> &vcData)
{
	std::ofstream fout(m_oRFIDPath.string().c_str(), std::ios::binary);
	fout.write(reinterpret_cast<const char*>(vcData.data()), vcData.size());
	fout.close();
}
