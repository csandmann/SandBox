/*
 * ReaderFile.h
 *
 *  Created on: Feb 10, 2019
 *      Author: check
 */

#ifndef INCLUDE_READERFILE_H_
#define INCLUDE_READERFILE_H_

#include "ReaderBase.h"
#include "../Logging/Logger.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

class ClReaderFile : public ClReaderBase
{
public:
	ClReaderFile(const StReaderConfig oReaderConfig);
	~ClReaderFile();
private:
	const std::vector<unsigned char> read() override;
	bool write(const std::vector<unsigned char> &stMsg) override;
	const fs::path m_oRFIDPath;
	const StReaderConfig m_oReaderConfig;
	ClLogger m_oLogger;
};


#endif /* INCLUDE_READERFILE_H_ */
