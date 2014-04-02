#ifndef TIME_SHIFT_ALG_H
#define TIME_SHIFT_ALG_H

#include "LafKernel/AlgBase.h"
#include "LafKernel/PhyEventBuf.h"
#include "TH1I.h"

class TimeShiftAlg : public AlgBase
{
    public :

	TimeShiftAlg(const std::string& name);
	virtual ~TimeShiftAlg();

	virtual bool initialize();
	virtual bool execute();
	virtual bool finalize();

    private :

	PhyEventBuf* m_buffer;
	PhyEventBuf* m_muonBuf;

	int   m_tw;
	int   m_stdDet;
	int   m_mulCut;
	bool  m_savePlot;

	int   m_site;
	TH1I* m_hist[7];

	std::string  m_rpath;
};

#endif
