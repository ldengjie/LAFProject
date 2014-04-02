#include "LafKernel/IPerEvent.h"
#include "LafKernel/PhyEvent.h"

void IPerEvent::link2PhyEvent(IPerEvent* ptr)
{
    PhyEvent::s_perEvt = ptr;
}
