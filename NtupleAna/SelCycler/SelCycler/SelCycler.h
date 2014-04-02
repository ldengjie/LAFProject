#ifndef SEL_CYCLER_H
#define SEL_CYCLER_H

#include "LafKernel/NormCycler.h"

class ITagBase;
class HallsTimeSync;

class SelCycler : public NormCycler
{
    public :

	SelCycler(const std::string& name);
	virtual ~SelCycler();

    private :

	virtual PhyEvent* theNext();

	bool  m_keepFirstEvtEachFile;

	HallsTimeSync* m_hallsSync;

	std::vector<ITagBase*>  m_tags;
};

#endif
