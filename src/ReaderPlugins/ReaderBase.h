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

struct StReaderConfig
{
	std::chrono::milliseconds nReaderDelay;
};

class ClReaderBase
{
public:
	ClReaderBase(const StReaderConfig *poConfig);
	virtual ~ClReaderBase(){};
	void start();
	void stop();
	StReaderMessage getMessage();
protected:
	virtual const StReaderMessage read() = 0;
	StReaderMessage m_stReaderMessage;
	const StReaderConfig *m_poConfig;
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
		std::this_thread::sleep_for(m_poConfig->nReaderDelay);
	}
};

inline
ClReaderBase::ClReaderBase(const StReaderConfig *poConfig) :
m_poConfig(poConfig),
m_bInterruptRequested(false)
{}

#endif /* INCLUDE_READERBASE_H_ */
