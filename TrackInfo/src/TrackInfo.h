#include  <iostream>
#include "LafKernel/AlgBase.h"
#include "LafKernel/GlobalVar.h"
#include "LiveTimeSvc/LiveTimeSvc.h"
#include "MuonVeto/MuonVeto.h"
#include "LafKernel/PhyEventBuf.h"
#include "TTree.h"
#include "TH1F.h"
#include "LafKernel/PhyEvent/CalibReadout.h"
using namespace std;

class TrackInfo : public AlgBase
{
  public:
    TrackInfo(const std::string& name);
    virtual ~TrackInfo(){}
    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

  private:
	/*
	LiveTimeSvc* liveTimeSvc;
	MuonVeto* muonVeto_l;
	TH1F* time2lastshowermuon1;
	TH1F* showermuonNum1;
    TTree* m_IWSMuonTree;
	*/
	PhyEventBuf* EvtBuffer;
	PhyEvent* CurEvent;
    bool ifcout;
    TH1F* h;
    TH1F* h1;
    TTree* t;
    float timeline[10][25][2];
    float timepoint[10][25][5];
    float charge[10][25];
    float energy;
    float hitcount[10][25];

	bool printEvt(PhyEvent* CurEvent);
};

