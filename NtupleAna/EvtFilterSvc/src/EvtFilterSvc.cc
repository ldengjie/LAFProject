#include "EvtFilterSvc/EvtFilterSvc.h"
#include "RidxSkimmer.h"
#include "EvtSkimmer.h"
#include "LafKernel/SvcFactory.h"

DECLARE_SERVICE(EvtFilterSvc);

EvtFilterSvc::EvtFilterSvc(const std::string& name)
    : SvcBase(name)
{
}

EvtFilterSvc::~EvtFilterSvc()
{
}

bool EvtFilterSvc::initialize()
{
    bool ridx = false;
    setOption("GenerateRidx", ridx);

    if ( ridx ) {
	m_skimmer = new RidxSkimmer( name() );
    }
    else {
	m_skimmer = new EvtSkimmer( name() );
    }

    return true;
}

bool EvtFilterSvc::finalize()
{
    delete m_skimmer;

    return true;
}

bool EvtFilterSvc::write(PhyEvent* evt)
{
    return m_skimmer->write(evt);
}
