#ifndef AD_SELECTOR_H
#define AD_SELECTOR_H

#include "LafKernel/EvtSelector.h"

class AdSelector : public EvtSelector
{
    public :

	AdSelector(const std::string& name);
	virtual ~AdSelector();

	virtual bool select(PhyEvent* evt);
};

#endif
