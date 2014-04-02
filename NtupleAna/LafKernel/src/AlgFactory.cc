#include "LafKernel/AlgFactory.h"
#include "LafKernel/AlgBase.h"
#include "LafKernel/AlgMgr.h"
#include "LafKernel/LafException.h"

AlgFactory::AlgFactory(const std::string& name, AlgBase* (*creator)(const std::string&))
{
    typedef AlgBase* (*AlgCreator)(const std::string&);

    std::map<std::string, AlgCreator>::iterator it = AlgMgr::elementCreatorMap.find(name);
    if ( it != AlgMgr::elementCreatorMap.end() ) {
	std::string msg = std::string("Duplicated algorithm definition: ") + name;
	throw LafException(msg);
    }
    AlgMgr::elementCreatorMap.insert(std::make_pair<std::string, AlgCreator>(name, creator));
}
