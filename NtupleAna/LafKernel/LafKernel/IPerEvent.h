#ifndef LAF_I_PER_EVENT_H
#define LAF_I_PER_EVENT_H

#include <string>

class DataSec;
class PhyEvent;

class IPerEvent
{
    public :

	virtual ~IPerEvent() {}

    protected :

	friend class PhyEvent;
	virtual DataSec* getObj(const std::string& path, PhyEvent* pEvt) = 0;

	void link2PhyEvent(IPerEvent* ptr);
};

#endif
