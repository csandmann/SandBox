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
	virtual ~ClReaderBase(){};
	void start();
	void stop();
	StReaderMessage getMessage();
private:
	virtual const StReaderMessage read() = 0;
	std::chrono::milliseconds m_nReaderDelay;
	StReaderMessage m_stReaderMessage;
	bool m_bInterruptRequested;
};


inline
StReaderMessage ClReaderBase::getMessage()
{
	return m_stReaderMessage;
};

inline
void ClReaderBase::stop()
{
	m_bInterruptRequested = true;
};

inline
void ClReaderBase::start()
{
	while (!m_bInterruptRequested)
	{
		m_stReaderMessage = read();
		std::this_thread::sleep_for(m_nReaderDelay);
	}
};

inline
ClReaderBase::ClReaderBase()
:m_nReaderDelay(1000),
m_bInterruptRequested(false)
{}

#endif /* INCLUDE_READERBASE_H_ */
