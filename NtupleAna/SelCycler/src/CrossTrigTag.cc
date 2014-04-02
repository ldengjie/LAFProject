#include "CrossTrigTag.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/ToolFactory.h"

#include <iostream>

DECLARE_TOOL(CrossTrigTag);

CrossTrigTag::CrossTrigTag(const std::string& name)
    : ITagBase(name)
{
    setOption("ApplyVeto", m_veto=false);
}

bool CrossTrigTag::tag(PhyEvent *event)
{
  //LogDebug << "in CrossTrigTag::tag()" << std::endl;
  // Tag cross trigger as bad event
  //LogDebug << event->m_trigType << "," << (event->m_trigType & 0x2) << std::endl;
  //if((event->m_trigType & 0x2) > 0) {
  if(event->m_trigType == 0x10000002) {
    event->m_crossTrigTag = true;
    event->m_good = false;
    return m_veto;
  }
  return false;
}
