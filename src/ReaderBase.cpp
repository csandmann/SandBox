#include "ReaderBase.h"
#include <thread>

ClReaderBase::ClReaderBase():
m_bInterruptRequested(false),
m_nReaderDelay(50)
{}

ClReaderBase::~ClReaderBase() {}

void ClReaderBase::start()
{
	while (!m_bInterruptRequested)
	{
		m_stReaderMessage = read();
		std::this_thread::sleep_for(m_nReaderDelay);
	}
}

void ClReaderBase::stop()
{
	m_bInterruptRequested = true;
}

StReaderMessage ClReaderBase::getMessage()
{
	return m_stReaderMessage;
}
