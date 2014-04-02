#include "ForceTrigTag.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/ToolFactory.h"

DECLARE_TOOL(ForceTrigTag);

ForceTrigTag::ForceTrigTag(const std::string& name)
    : ITagBase(name)
{
    setOption("ApplyVeto", m_veto=false);
}

bool ForceTrigTag::tag(PhyEvent *event)
{
  //LogDebug << "in ForceTrigTag::tag()" << std::endl;
  // Tag force trigger as bad event
  if(event->m_trigType == 0x10000020) {
    event->m_forceTrigTag = true;
    event->m_good = false;
    return m_veto;
  }
  return false;
}
