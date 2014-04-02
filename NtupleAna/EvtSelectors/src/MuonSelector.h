#ifndef MUON_SELECTOR_H
#define MUON_SELECTOR_H

#include "LafKernel/EvtSelector.h"

class MuonSelector : public EvtSelector
{
    public :

	MuonSelector(const std::string& name);
	virtual ~MuonSelector();

	virtual bool select(PhyEvent* evt);

    private :

	bool m_vetoRetrig;
};

#endif
