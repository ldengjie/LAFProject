#include "LafKernel/UserBuffer.h"

UserBuffer::UserBuffer(EvtSelector* selector)
    : m_selector(selector)
{
}

UserBuffer::~UserBuffer()
{
    delete m_selector;
}

bool UserBuffer::tryPushBack(PhyEvent* evt)
{
    if ( m_selector->select(evt) ) {
	m_dBuf.push_back(evt->GrabInstance());
	return true;
    }
    return false;
}

void UserBuffer::synchronize(PhyEventBuf* buf)
{
    PhyEventBuf::Iterator it = buf->begin();
    while ( !m_dBuf.empty() && m_dBuf[0]->m_trigTime < (*it)->m_trigTime ) {
	m_dBuf[0]->ReleaseInstance();
	m_dBuf[0] = 0;
	m_dBuf.pop_front();
    }
    unsigned int size = m_dBuf.size();
    m_iCur = size;
    if ( size > 0 ) {
	int curEntry = buf->curEvt()->m_entry;
	const TTimeStamp& curTime = buf->curEvt()->m_trigTime;
	while ( --m_iCur != 0 && m_dBuf[m_iCur]->m_entry != curEntry ) {
	    if ( m_dBuf[m_iCur]->m_trigTime < curTime ) break;
	}
	if ( m_dBuf[m_iCur]->m_entry != curEntry ) {
	    m_iCur = size;
	}
    }
}
