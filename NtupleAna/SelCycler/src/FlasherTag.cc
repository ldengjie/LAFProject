#include "FlasherTag.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/ToolFactory.h"
#include "LafKernel/PhyEvent/CalibReadout.h"

DECLARE_TOOL(FlasherTag);

FlasherTag::FlasherTag(const std::string& name)
    : ITagBase(name)
{
    setOption("ApplyVeto", m_veto=false);
    setOption("RatioMaxQCut", m_ratioMaxQ_Cut);
    setOption("Quadrantcut", m_quadrant_Cut);
    OptionParser::setOption(name, "calibPMTFlasherCut", m_2inchFlasher_Cut=0.);
}

bool FlasherTag::tag(PhyEvent *event)
{
  //LogDebug << "in FlasherTag::tag()" << std::endl;
  if(event->isAD()) {
    if((event->m_quadrant/m_quadrant_Cut)*(event->m_quadrant/m_quadrant_Cut) +
        (event->m_maxQ/m_ratioMaxQ_Cut)*(event->m_maxQ/m_ratioMaxQ_Cut) > 1) {
      event->m_flasherTag = true;
      event->m_good = false;
    }

    // 2inch PMT flasher
    if(m_2inchFlasher_Cut!=0.) {
      if(event->m_maxQ_2inchPMT>m_2inchFlasher_Cut && event->energy() <20.) {
        event->m_good = false;
        event->m_2inchFlasherTag = true;
        LogDebug << "detector, ipmt, charge, energy : " << event->m_det << ", " << event->m_Column_2inchPMT << ", " << event->m_maxQ_2inchPMT << ", " <<event->energy()<< std::endl;;
      }
    }

    if(event->m_flasherTag || event->m_2inchFlasherTag) return m_veto;
  }

  return false;
}
