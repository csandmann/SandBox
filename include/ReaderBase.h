/*
 * ReaderBase.hpp
 *
 *  Created on: Feb 8, 2019
 *      Author: check
 */

#ifndef INCLUDE_READERBASE_H_
#define INCLUDE_READERBASE_H_

#include <future>
#include <chrono>

enum class EReaderStatus
{
	DETECTED = 0,
	EMPTY = 1
};

struct StReaderMessage
{
	EReaderStatus eStatus;
	int nKey;
};

class ClReaderBase
{
public:
	ClReaderBase();
	virtual ~ClReaderBase() = 0;
	void start();
	void stop();
	StReaderMessage getMessage();
private:
	virtual StReaderMessage read() = 0;
	std::chrono::milliseconds m_nReaderDelay;
	StReaderMessage m_stReaderMessage;
	bool m_bInterruptRequested;
};


#endif /* INCLUDE_READERBASE_H_ */
