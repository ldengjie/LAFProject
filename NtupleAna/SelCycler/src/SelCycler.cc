#include "SelCycler/SelCycler.h"
#include "HallsTimeSync.h"
#include "SelCycler/ITagBase.h"
#include "LafKernel/SvcFactory.h"
#include "LafKernel/ToolMgr.h"
#include "LafKernel/LafException.h"

DECLARE_SERVICE(SelCycler);

SelCycler::SelCycler(const std::string& name)
    : NormCycler(name)
{
    m_name = "SelCycler";  //to avoid naming confusion
    // Keep the first event of each file for livetime calculation
    setOption("KeepFirstEvtEachFile", m_keepFirstEvtEachFile = false);

    bool fromMuon, hallsSync;
    setOption("StartFromFirstMuon",   fromMuon = false);
    setOption("HallsTimeSync",        hallsSync = false);

    m_hallsSync = hallsSync ? new HallsTimeSync : 0;

    std::vector<std::string> tags;
    setOption("Tags", tags);

    if ( fromMuon ) tags.push_back("FirstMuonTag");

    for(std::vector<std::string>::iterator it = tags.begin(); it != tags.end(); ++it) {
	ITagBase* tag = static_cast<ITagBase*>(ToolMgr::get(*it, true));
	if ( tag != 0 ) {
	    m_tags.push_back(tag);
	}
	else {
	    std::string msg = std::string("@ SelCycler constructor: invalid tool ") + *it;
	    throw LafException(msg);
	}
    }
}

SelCycler::~SelCycler()
{
}

PhyEvent* SelCycler::theNext()
{
    int nTag = m_tags.size();
    PhyEvent* _next = 0;
    while ( (_next=(PhyEvent*)(m_is->next())) != 0 ) {
	if ( m_hallsSync && ! m_hallsSync->validate(_next) ) {
	    _next->ReleaseInstance();
	    continue;
	}
	for ( int i = 0; i < nTag; ++i ) {
	    if ( m_tags[i]->tag(_next) ) {
		if ( m_keepFirstEvtEachFile && _next->m_localEntry == 0 ) continue;
		_next->ReleaseInstance();  //delete _next;
		_next = 0;
		break;
	    }
	}
	if ( _next != 0 ) break;
    }
    return _next;
}
