#include "FanCycler/FanCycler.h"
#include "FanCycler/IFanTagBase.h"
#include "LafKernel/SvcFactory.h"
#include "LafKernel/SvcMgr.h"
#include "LafKernel/ToolMgr.h"
#include "LafKernel/LafException.h"

DECLARE_SERVICE(FanCycler);

FanCycler::FanCycler(const std::string& name)
    : NormCycler(name)
{
    m_name = "FanCycler";  //to avoid naming confusion

    ToolMgr* _toolMgr = ToolMgr::instance();
    std::vector<std::string> tags;
    setOption("Tags", tags);
    for(std::vector<std::string>::iterator it = tags.begin(); it != tags.end(); ++it) {
	IFanTagBase* tag = _toolMgr->get<IFanTagBase>(*it, true);
	if ( tag != 0 ) {
	    m_tags.push_back(tag);
	}
	else {
	    std::string msg = std::string("@ FanCycler constructor: invalid tool ") + *it;
	    throw LafException(msg);
	}
    }
}

FanCycler::~FanCycler()
{
}

bool FanCycler::initialize()
{
    if ( NormCycler::initialize() ) {
	m_is = SvcMgr::get<ICycler>("SelCycler");
	m_buffer = SvcMgr::get<PhyEventBuf>("SelCycler");
	return true;
    }
    return false;
}

PhyEvent* FanCycler::theNext()
{
    int nTag = m_tags.size();
    PhyEvent* _next = 0;
    while ( (_next=(PhyEvent*)(m_is->next())) != 0 ) {
	for ( int i = 0; i < nTag; ++i ) {
	    if ( m_tags[i]->tag(m_buffer) ) {
		_next->ReleaseInstance();  //delete _next;
		_next = 0;
		break;
	    }
	}
	if ( _next != 0 ) break;
    }
    return _next;
}
