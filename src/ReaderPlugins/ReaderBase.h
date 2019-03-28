/*
 * ReaderBase.hpp
 *
 *  Created on: Feb 8, 2019
 *      Author: check
 */

#ifndef INCLUDE_READERBASE_H_
#define INCLUDE_READERBASE_H_

#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include "ReaderMessage.h"

struct StReaderConfig
{
	std::chrono::milliseconds nReadInterval;
};

class ClReaderBase
{
public:
	ClReaderBase(const StReaderConfig *poConfig);
	virtual ~ClReaderBase(){};
	void start();
	void stop();
	ReaderMessage::StMessage getMessage();
	void requestWrite(const ReaderMessage::StCardData &stCardData);
protected:
	virtual const std::vector<unsigned char> read() = 0;
	virtual bool write(const std::vector<unsigned char> &stMessage) = 0;
	ReaderMessage::StMessage m_stReaderMessage;
	ReaderMessage::StCardData m_stToWrite;
	const StReaderConfig *m_poBaseConfig;
	bool m_bInterruptRequested;
};



#endif /* INCLUDE_READERBASE_H_ */
