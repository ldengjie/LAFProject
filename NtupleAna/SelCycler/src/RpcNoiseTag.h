#ifndef RPCNOISETAG_H
#define RPCNOISETAG_H

#include "SelCycler/ITagBase.h"

class RpcNoiseTag : public ITagBase
{
  public:
    RpcNoiseTag(const std::string& name);
    virtual ~RpcNoiseTag(){}
    virtual bool tag(PhyEvent* event);

};

#endif //RPCNOISETAG_H
