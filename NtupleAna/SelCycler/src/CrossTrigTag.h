#ifndef CROSSTRIGTAG_H
#define CROSSTRIGTAG_H

#include "SelCycler/ITagBase.h"

class CrossTrigTag : public ITagBase
{
  public:
    CrossTrigTag(const std::string& name);
    virtual ~CrossTrigTag(){}
    virtual bool tag(PhyEvent* event);

};

#endif //CROSSTRIGTAG_H
