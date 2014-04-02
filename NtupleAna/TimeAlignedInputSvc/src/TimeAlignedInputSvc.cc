#include "TimeAlignedInputSvc/TimeAlignedInputSvc.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/SvcMgr.h"
#include "LafKernel/SvcFactory.h"
#include "LafKernel/LafException.h"
#include <fstream>
#include <limits>
#include <cstdlib>

DECLARE_SERVICE(TimeAlignedInputSvc);

TimeAlignedInputSvc::TimeAlignedInputSvc(const std::string& name)
    : SvcBase(name),
      m_is(0),
      m_window(0)
{
    m_name = "TimeAlignedInputSvc";
}

TimeAlignedInputSvc::~TimeAlignedInputSvc()
{
}

bool TimeAlignedInputSvc::initialize()
{
    m_is = SvcMgr::get<ICycler>("ReconInputSvc");

    int run = 0, _run = -1;
    setOption("RunNumber", run);

    if ( run != 0 ) {
	std::string tfile;
	setOption("TimeShiftFromFile", tfile);

	std::ifstream ifs(tfile.c_str());
	ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while ( ifs.good() ) {
	    ifs >> _run;
	    if ( _run == run ) {
		for ( int i = 0; i < 7; ++i ) {
		    ifs >> m_dt[i];
		    LogDebug << "Det[" << i+1 << "] time shift: " << m_dt[i] << std::endl;
		    if ( abs(m_dt[i])*2 > m_window ) m_window = abs(m_dt[i])*2;
		}
		m_window *= 1.e-9;
		LogDebug << "TimeWindow " << m_window << std::endl;
		break;
	    }
	    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
    }

    if ( _run != run ) {
	LogFatal << "Invalid RunNumber: " << run << std::endl;
	return false;
    }

    return true;
}

bool TimeAlignedInputSvc::finalize()
{
    return true;
}

EvtObject* TimeAlignedInputSvc::next(int nth)
{
    while ( m_evts.size() < 2 || (m_evts.back()->m_trigTime-m_evts.front()->m_trigTime) < m_window ) {
	PhyEvent* newEvt = static_cast<PhyEvent*>(m_is->next());
	if ( newEvt == 0 ) break;
	newEvt->m_trigTime.Add(TTimeStamp(0, -m_dt[newEvt->m_det-1]));
	//insert newEvt to the list
	std::list<PhyEvent*>::reverse_iterator it = m_evts.rbegin();
	for ( ; it != m_evts.rend(); ++it ) {
	    if ( (*it)->m_trigTime <= newEvt->m_trigTime ) {
		break;
	    }
	}
	m_evts.insert(it.base(), newEvt);
    }

    if ( m_evts.empty() ) return 0;

    PhyEvent* first = m_evts.front();
    m_evts.pop_front();
    //std::cout << first->m_entry << " : " << first->m_trigTime << std::endl;
    return first;
}
