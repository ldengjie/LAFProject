#ifndef FORCETRIGTAG_H
#define FORCETRIGTAG_H

#include "SelCycler/ITagBase.h"

class ForceTrigTag : public ITagBase
{
  public:
    ForceTrigTag(const std::string& name);
    virtual ~ForceTrigTag(){}
    virtual bool tag(PhyEvent* event);

};

#endif //FORCETRIGTAG_H
