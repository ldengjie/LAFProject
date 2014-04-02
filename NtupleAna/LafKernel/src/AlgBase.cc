#include "LafKernel/AlgBase.h"
#include "LafKernel/SvcMgr.h"

SvcBase* AlgBase::service(const std::string& name)
{
    return SvcMgr::get(name);
}

TOutputSvc* AlgBase::ntupleSvc()
{
    return SvcMgr::get<TOutputSvc>("TOutputSvc");
}
