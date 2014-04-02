#ifndef HALLS_SYNC_H
#define HALLS_SYNC_H

#include "TTimeStamp.h"
#include <list>
#include <utility>

class PhyEvent;

class HallsTimeSync
{
    public :
	HallsTimeSync();

	bool validate(PhyEvent* evt);

    private :

	typedef std::list<std::pair<TTimeStamp, TTimeStamp> > Periods;

	TTimeStamp  m_pEnd;
	Periods     m_periods;
};

#endif
