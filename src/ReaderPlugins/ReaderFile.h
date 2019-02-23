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
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


class ClReaderFile : public ClReaderBase
{
public:
	ClReaderFile(const StReaderConfig oReaderConfig);
	~ClReaderFile();
private:
	const StReaderMessage read() override;
	const fs::path m_oRFIDPath;
	const StReaderConfig m_oReaderConfig;
	ClLogger m_oLogger;
};


#endif /* INCLUDE_READERFILE_H_ */
