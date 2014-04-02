#ifndef I_FAN_TAGBASE_H
#define I_FAN_TAGBASE_H

#include "LafKernel/PhyEventBuf.h"

class IFanTagBase
{
    public:

	virtual ~IFanTagBase(){}
	virtual bool tag(PhyEventBuf* buf) = 0;

    protected :

	bool m_veto;
};

#endif //I_FAN_TAGBASE_H
