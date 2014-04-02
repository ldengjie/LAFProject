#ifndef ANENERGYTAG_H
#define ANENERGYTAG_H

#include "SelCycler/ITagBase.h"

class AdEnergyTag : public ITagBase
{
  public:
    AdEnergyTag(const std::string& name);
    virtual ~AdEnergyTag(){}

    virtual bool tag(PhyEvent* event);

  private :
    float  m_adEngThr;
};

#endif //ANENERGYTAG_H
