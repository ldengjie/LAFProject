#include "RpcNoiseTag.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/ToolFactory.h"

DECLARE_TOOL(RpcNoiseTag);

RpcNoiseTag::RpcNoiseTag(const std::string& name)
    : ITagBase(name)
{
  setOption("ApplyVeto", m_veto=false);
}

bool RpcNoiseTag::tag(PhyEvent *event)
{
  //LogDebug << "in RpcNoiseTag::tag()" << std::endl;
  // Tag pure 2/4 RPC event as noise
  if(event->m_trigType == 0x10020000) {
    event->m_rpcNoiseTag = true;
    event->m_good = false;
    return m_veto;
  }
  return false;
}
