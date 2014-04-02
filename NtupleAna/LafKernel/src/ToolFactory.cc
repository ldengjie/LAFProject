#include "LafKernel/ToolFactory.h"
#include "LafKernel/ToolBase.h"
#include "LafKernel/ToolMgr.h"
#include "LafKernel/LafException.h"

ToolFactory::ToolFactory(const std::string& name, ToolBase* (*creator)(const std::string&))
{
    typedef ToolBase* (*ToolCreator)(const std::string&);

    std::map<std::string, ToolCreator>::iterator it = ToolMgr::elementCreatorMap.find(name);
    if ( it != ToolMgr::elementCreatorMap.end() ) {
	std::string msg = std::string("Duplicated tool definition: ") + name;
	throw LafException(msg);
    }
    ToolMgr::elementCreatorMap.insert(std::make_pair<std::string, ToolCreator>(name, creator));
}
