#include "LafKernel/UserBufFactory.h"
#include "LafKernel/UserBufMgr.h"
#include "LafKernel/LafException.h"

UserBufFactory::UserBufFactory(const std::string& name, UserBuffer* (*creator)(const std::string&))
{
    typedef UserBuffer* (*BufferCreator)(const std::string&);

    std::map<std::string, BufferCreator>::iterator it = UserBufMgr::elementCreatorMap.find(name);
    if ( it != UserBufMgr::elementCreatorMap.end() ) {
	std::string msg = std::string("Duplicated EvtSelector definition: ") + name;
	throw LafException(msg);
    }
    UserBufMgr::elementCreatorMap.insert(std::make_pair<std::string, BufferCreator>(name, creator));
}
