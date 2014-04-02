#ifndef CONT_CYCLER_H
#define CONT_CYCLER_H

#include "LafKernel/PhyEventBuf.h"
#include "LafKernel/SvcBase.h"
#include "LafKernel/ICycler.h"

class NormCycler;

class ContCycler : public SvcBase,
                   public PhyEventBuf,
		   virtual public ICycler
{
    public :

	ContCycler(const std::string& name);
	virtual ~ContCycler();

	virtual bool initialize();
	virtual bool finalize();

	virtual PhyEvent* next(int /*nth = 1*/);


    protected :

	PhyEvent*    nextAD();

	int          m_shift;
	NormCycler*  m_is;
	std::string  m_isName;
};

#endif
