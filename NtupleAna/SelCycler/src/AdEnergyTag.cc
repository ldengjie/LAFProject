#include "AdEnergyTag.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/ToolFactory.h"

DECLARE_TOOL(AdEnergyTag);

AdEnergyTag::AdEnergyTag(const std::string& name)
    : ITagBase(name)
{
    setOption("ApplyVeto", m_veto=false);
    setOption("AdEnergyThreshold", m_adEngThr);
}

bool AdEnergyTag::tag(PhyEvent *event)
{
  //LogDebug << "in AdEnergyTag::tag()" << std::endl;
  if(event->isAD()) {
    if(event->energy() <m_adEngThr) {
      event->m_adLowEnergyTag = true;
      event->m_good = false;
      return m_veto;
    }
  }

  return false;
}
