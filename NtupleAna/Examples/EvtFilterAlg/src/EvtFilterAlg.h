#ifndef EVT_FILTER_ALG_H
#define EVT_FILTER_ALG_H

#include "LafKernel/AlgBase.h"
#include "LafKernel/PhyEventBuf.h"

class EvtFilterSvc;

class EvtFilterAlg : public AlgBase
{
    public :

	EvtFilterAlg(const std::string& name);
	virtual ~EvtFilterAlg();

	virtual bool initialize();
	virtual bool execute();
	virtual bool finalize();

    private :

	PhyEventBuf*   m_buffer;

	EvtFilterSvc*  m_filter;
};

#endif
