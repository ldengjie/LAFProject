#include "FirstMuonTag.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/ToolFactory.h"

DECLARE_TOOL(FirstMuonTag);

FirstMuonTag::FirstMuonTag(const std::string& name)
    : ITagBase(name),
      m_active(true)
{
}

bool FirstMuonTag::tag(PhyEvent* event)
{
    if ( m_active ) {
	if ( ! event->isRPC() && event->isMuon() ) {
	    m_active = false;
	}
    }

    return m_active;
}
