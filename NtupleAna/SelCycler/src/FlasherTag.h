#ifndef FLASHERTAG_H
#define FLASHERTAG_H

#include "SelCycler/ITagBase.h"

class FlasherTag : public ITagBase
{
  public:
    FlasherTag(const std::string& name);
    virtual ~FlasherTag(){}

    virtual bool tag(PhyEvent* event);

  private :
    float  m_ratioMaxQ_Cut;
    float  m_quadrant_Cut;
    float  m_2inchFlasher_Cut;
};

#endif //FLASHERTAG_H
