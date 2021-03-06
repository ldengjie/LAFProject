#include "LafKernel/AlgMgr.h"
#include "LafKernel/LafLog.h"

AlgMgr* AlgMgr::_instance = 0;

AlgMgr* AlgMgr::instance()
{
    if ( _instance == 0 ) {
	_instance = new AlgMgr("AlgMgr");
    }
    return _instance;
}

AlgMgr::AlgMgr(const std::string& objname)
    : ElementMgr<AlgBase>(objname)
{
    for ( std::list<AlgBase*>::iterator it = m_elements.begin(); it != m_elements.end(); ++it ) {
	LogInfo << "Add  algorithm  : " << (*it)->name() << std::endl;
    }
}

bool AlgMgr::execute()
{
    for ( std::list<AlgBase*>::iterator it = m_elements.begin(); it != m_elements.end(); ++it ) {
	if ( ! (*it)->execute() ) {
	    LogError << "failed to execute algorithm " << (*it)->name() << std::endl;
	    return false;
	}
    }
    return true;
}

