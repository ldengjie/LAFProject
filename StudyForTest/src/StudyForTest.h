#include  <iostream>
#include "LafKernel/AlgBase.h"
#include "LafKernel/GlobalVar.h"
#include "LiveTimeSvc/LiveTimeSvc.h"
#include "MuonVeto/MuonVeto.h"
#include "LafKernel/PhyEventBuf.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "LafKernel/PhyEvent/CalibReadout.h"
#include  "TMath.h"
using namespace std;

class StudyForTest : public AlgBase
{
  public:
    StudyForTest(const std::string& name);
    virtual ~StudyForTest(){}
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
    TString histName;
	PhyEventBuf* EvtBuffer;
	PhyEvent* CurEvent;

	bool printEvt(PhyEvent* CurEvent);
    bool singleTrigger4Li(PhyEvent* CurEvent);
        TTree* m_TriggerTree;
        float x,y,z,energy;
        int DetId;
    bool muonEnergyCorrection(PhyEvent* CurEvent);
        TH1F* AdMuonEnergy;
        TH1F* AdMuonEnergyAfterCor;
        TH2F* AdMuonEnergyVsAfterCor;
        double maxPE;
        double SatPENum;
        double alpha;
        //double obsPE[8][10][10][10];
        //double findRealPE(double PENum);
        
};
