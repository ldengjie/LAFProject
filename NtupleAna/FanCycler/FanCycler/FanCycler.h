#ifndef FAN_CYCLER_H
#define FAN_CYCLER_H

#include "LafKernel/NormCycler.h"
#include "LafKernel/PhyEventBuf.h"

class IFanTagBase;

class FanCycler : public NormCycler
{
    public :

	FanCycler(const std::string& name);
	virtual ~FanCycler();

	virtual bool initialize();

    private :

	virtual PhyEvent* theNext();

	PhyEventBuf*               m_buffer;
	std::vector<IFanTagBase*>  m_tags;
};

#endif
