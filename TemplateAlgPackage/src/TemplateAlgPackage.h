#include  <iostream>
#include "LafKernel/AlgBase.h"
#include "LafKernel/GlobalVar.h"
#include "LiveTimeSvc/LiveTimeSvc.h"
#include "MuonVeto/MuonVeto.h"
#include "LafKernel/PhyEventBuf.h"
#include "TTree.h"
#include "TH1F.h"
using namespace std;

class TemplateAlgPackage : public AlgBase
{
  public:
    TemplateAlgPackage(const std::string& name);
    virtual ~TemplateAlgPackage(){}
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

	bool printEvt(PhyEvent* CurEvent);
};

