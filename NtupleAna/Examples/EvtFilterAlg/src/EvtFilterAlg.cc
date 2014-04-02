#include "EvtFilterAlg.h"
#include "EvtFilterSvc/EvtFilterSvc.h"
#include "LafKernel/PhyEvent/RecReadout.h"
#include "LafKernel/PhyEvent/RecRpcReadout.h"
#include "LafKernel/PhyEvent/CalibStats.h"
#include "LafKernel/AlgFactory.h"

DECLARE_ALGORITHM(EvtFilterAlg);

EvtFilterAlg::EvtFilterAlg(const std::string& name)
    : AlgBase(name)
{
}

EvtFilterAlg::~EvtFilterAlg()
{
}

bool EvtFilterAlg::initialize()
{
    m_filter = dynamic_cast<EvtFilterSvc*>(service("EvtFilterSvc"));
    if ( m_filter == 0 ) {
	LogError << "Failed to get EvtFilterSvc !" << std::endl;
	return false;
    }

    m_buffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));

    return true;
}

bool EvtFilterAlg::execute()
{
    PhyEvent *event = m_buffer->curEvt();
    LogDebug << event->m_fileNum << "-" << event->m_localEntry
	     << ": " << event->isMuon() << std::endl;

    if ( event->isAD() && event->m_forceTrigTag ) {
	m_filter->write(event);
    }

    return true;
}

bool EvtFilterAlg::finalize()
{
    return true;
}
