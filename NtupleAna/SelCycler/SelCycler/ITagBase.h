#ifndef I_TAGBASE_H
#define I_TAGBASE_H

#include "LafKernel/ToolBase.h"

class PhyEvent;

class ITagBase : public ToolBase
{
  public:
      ITagBase(const std::string& name) : ToolBase(name) {}
      virtual ~ITagBase(){}

      virtual bool tag(PhyEvent* event) = 0;

  protected :
      bool m_veto;
};

#endif //TAGBASE_H
