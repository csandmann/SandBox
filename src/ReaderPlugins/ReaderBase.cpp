#include "ReaderBase.h"
#include <iostream>


ReaderMessage::StMessage ClReaderBase::getMessage()
{
	return m_stReaderMessage;
}

void ClReaderBase::stop()
{
	m_bInterruptRequested = true;
}

void ClReaderBase::start()
{
	while (!m_bInterruptRequested)
	{
		//read
		m_stReaderMessage = ReaderMessage::deserializeCardData(read());
		//write
		if (m_stToWrite.vcPlayerMessage.size() > 0)
		{
			std::cout << "nVersionNumber: " << m_stToWrite.nVersionNumber << std::endl;
			bool bSuccess = write(ReaderMessage::serializeCardData(m_stToWrite));
			if (bSuccess) {
				ReaderMessage::resetCardData(m_stToWrite);	
			}
		}
		std::this_thread::sleep_for(m_poBaseConfig->nReadInterval);
	}
}

ClReaderBase::ClReaderBase(const StReaderConfig *poConfig) :
m_poBaseConfig(poConfig),
m_bInterruptRequested(false)
{}

void ClReaderBase::requestWrite(const ReaderMessage::StCardData &stMsg)
{
	m_stToWrite = ReaderMessage::StCardData(stMsg);
}
