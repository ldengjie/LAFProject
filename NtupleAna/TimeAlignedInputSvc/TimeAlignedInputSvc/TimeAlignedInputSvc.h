#ifndef TIME_ALIGNED_INPUT_SVC_H
#define TIME_ALIGNED_INPUT_SVC_H

#include "LafKernel/SvcBase.h"
#include "LafKernel/ICycler.h"
#include <list>

class PhyEvent;

class TimeAlignedInputSvc : public SvcBase,
                            virtual public ICycler
{
    public :

	TimeAlignedInputSvc(const std::string& name);
	virtual ~TimeAlignedInputSvc();

	virtual bool initialize();
	virtual bool finalize();

	virtual EvtObject* next(int nth = 1);


    private :

	ICycler* m_is;
	int      m_dt[7];

	double   m_window;
	std::list<PhyEvent*> m_evts;
};

#endif
