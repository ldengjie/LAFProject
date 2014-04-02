#include "LafKernel/SvcFactory.h"
#include "LafKernel/SvcBase.h"
#include "LafKernel/SvcMgr.h"
#include "LafKernel/LafException.h"

SvcFactory::SvcFactory(const std::string& name, SvcBase* (*creator)(const std::string&))
{
    typedef SvcBase* (*SvcCreator)(const std::string&);

    std::map<std::string, SvcCreator>::iterator it = SvcMgr::elementCreatorMap.find(name);
    if ( it != SvcMgr::elementCreatorMap.end() ) {
	std::string msg = std::string("Duplicated service definition: ") + name;
	throw LafException(msg);
    }
    SvcMgr::elementCreatorMap.insert(std::make_pair<std::string, SvcCreator>(name, creator));
}
