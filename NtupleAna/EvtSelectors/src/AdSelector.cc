#include "AdSelector.h"
#include "LafKernel/UserBufFactory.h"
#include "LafKernel/PhyEvent.h"

DECLARE_BUFFER_WITH_SELECTOR(AdSelector);

AdSelector::AdSelector(const std::string& name)
    : EvtSelector(name)
{
}

AdSelector::~AdSelector()
{
}

bool AdSelector::select(PhyEvent* evt)
{
    return evt->isAD();
}
