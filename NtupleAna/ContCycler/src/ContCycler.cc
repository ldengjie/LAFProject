#include "ContCycler/ContCycler.h"
#include "LafKernel/NormCycler.h"
#include "LafKernel/SvcMgr.h"
#include "LafKernel/SvcFactory.h"

DECLARE_SERVICE(ContCycler);

ContCycler::ContCycler(const std::string& name)
    : SvcBase(name),
      m_shift(1)
{
    m_name = "ContCycler";  //to avoid naming confusion
    m_iCur = 0;
    setOption("Input", m_isName = "SelCycler");
}

ContCycler::~ContCycler()
{
}

bool ContCycler::initialize()
{
    m_is = SvcMgr::get<NormCycler>(m_isName, true);
    if ( m_is != 0 ) {
	LogDebug << "Set the InputSvc: " << m_isName << " = " << m_is << std::endl;
	return true;
    }
    return false;
}

bool ContCycler::finalize()
{
    PhyEventBuf::clear();
    return true;
}

PhyEvent* ContCycler::next(int /*nth*/)
{
    PhyEventBuf::clear();
    PhyEvent* first = (PhyEvent*)(m_is->next(m_shift));
    while ( first != 0 && !first->isAD() ) first = (PhyEvent*)(m_is->next());
    m_shift = 0;

    while ( first != 0 ) {
	PhyEvent* aAD = first;
	PhyEvent* bAD = nextAD();
	while ( bAD != 0 && (bAD->m_trigTime-aAD->m_trigTime) < 2e-4 ) {
	    m_dBuf.push_back(bAD->GrabInstance());
	    aAD = bAD;
	    bAD = nextAD();
	}
	if ( m_dBuf.size() != 0 ) {
	    m_dBuf.push_front(first->GrabInstance());
	    break;
	}
	first = (PhyEvent*)(m_is->next(m_shift));
	m_shift = 0;
    }

    return first;
}

PhyEvent* ContCycler::nextAD()
{
    PhyEventBuf::Iterator end = m_is->end();
    for ( PhyEventBuf::Iterator it = m_is->current()+m_shift+1; it != end; ++it ) {
	++m_shift;
	if ( (*it)->isAD() ) return (*it);
    }

    PhyEvent* nAD = 0;
    do {
	nAD = m_is->expand();
	++m_shift;
    } while ( nAD != 0 && !nAD->isAD() );

    return nAD;
}
