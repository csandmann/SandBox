/*
 * ReaderFile.h
 *
 *  Created on: Feb 10, 2019
 *      Author: check
 */

#ifndef INCLUDE_READERFILE_H_
#define INCLUDE_READERFILE_H_

#include "ReaderBase.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


class ClReaderFile : public ClReaderBase
{
public:
	ClReaderFile();
	~ClReaderFile();
private:
	StReaderMessage read();
	const fs::path m_oRFIDPath;
};


#endif /* INCLUDE_READERFILE_H_ */
