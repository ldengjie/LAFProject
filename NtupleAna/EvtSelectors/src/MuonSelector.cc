#include "MuonSelector.h"
#include "LafKernel/UserBufFactory.h"
#include "LafKernel/PhyEvent.h"

DECLARE_BUFFER_WITH_SELECTOR(MuonSelector);

MuonSelector::MuonSelector(const std::string& name)
    : EvtSelector(name)
{
    setOption("VetoRetrigger", m_vetoRetrig = false);
}

MuonSelector::~MuonSelector()
{
}

bool MuonSelector::select(PhyEvent* evt)
{
    if ( evt->isMuon() ) {
	if ( evt->m_det > 4 || ! m_vetoRetrig || evt->m_MiddleTimeRMS < 50 ) {
	    return true;
	}
    }
    return false;
}
