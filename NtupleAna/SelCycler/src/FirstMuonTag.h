#ifndef FIRST_MUON_TAG_H
#define FIRST_MUON_TAG_H

#include "SelCycler/ITagBase.h"

class FirstMuonTag : public ITagBase
{
    public :

	FirstMuonTag(const std::string& name);
	virtual ~FirstMuonTag() {}

	virtual bool tag(PhyEvent* event);

    private :

	bool m_active;
};

#endif
