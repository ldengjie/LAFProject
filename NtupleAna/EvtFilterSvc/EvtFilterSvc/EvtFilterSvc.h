#ifndef LAF_EVT_FILTER_SVC_H
#define LAF_EVT_FILTER_SVC_H

#include "LafKernel/SvcBase.h"

class PhyEvent;
class ISkimmer;

class EvtFilterSvc : public SvcBase
{
    public :

	EvtFilterSvc(const std::string& name);
	virtual ~EvtFilterSvc();

	virtual bool initialize();
	virtual bool finalize();

	bool write(PhyEvent* evt);


    private :

	ISkimmer* m_skimmer;
};

#endif
