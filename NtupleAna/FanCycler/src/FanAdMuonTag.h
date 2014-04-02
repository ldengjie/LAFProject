#ifndef FAN_AD_MUONTAG_H
#define FAN_AD_MUONTAG_H

#include "FanCycler/IFanTagBase.h"
#include "LafKernel/ToolBase.h"
#include    "LafKernel/PhyEvent/CalibReadout.h"
#include    "TMath.h"

class FanAdMuonTag : public IFanTagBase, public ToolBase
{
    public :

	FanAdMuonTag(const std::string& name);
	virtual ~FanAdMuonTag(){}

	virtual bool tag(PhyEventBuf* buf);


    private :

	double  m_adMuonThr;
	double  m_showerMuonThr;
    bool m_cor;
    bool m_adMuonCut;
	std::vector<double>  m_window;
};

#endif //MUONTAG_H
