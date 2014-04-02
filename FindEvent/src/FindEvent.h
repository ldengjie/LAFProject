#include  <iostream>
#include  "TMath.h"
#include  <string>
#include  "MuonVeto/MuonVeto.h"
#include  "LafKernel/PhyEvent.h"
#include  <vector>
#include  "LafKernel/OptionParser.h"
#include  "LafKernel/AlgBase.h"
#include  "LafKernel/DataBuffer.h"
#include  "LafKernel/GlobalVar.h"
#include  "LafKernel/LafLog.h"
using namespace std;  

class FindEvent : public AlgBase
{
  public:
	FindEvent(const std::string& name);
	virtual ~FindEvent(){}

	virtual bool initialize();
	virtual bool execute();
	virtual bool finalize();
	virtual bool IsIbd(PhyEvent* CurEvent);
	virtual bool FindRunInfo(PhyEvent* CurEvent);
	virtual bool PreEvtInfo(PhyEvent* CurEvent);
	virtual bool printEvt(PhyEvent* CurEvent);
	virtual bool NextEvtInfo(PhyEvent* CurEvent);
	virtual bool Time2PreMuon(PhyEvent* CurEvent);
  private:

	PhyEventBuf* EvtBuffer;
	PhyEvent* CurEvent;
	MuonVeto* muonVeto_l;
	vector<PhyEvent*> AdBuf;
	vector<PhyEvent*> AdEvtBuf[4];
    int PromptFileNum;
    int PromptLocalEntry;
    int PromptTrigNum;
    int DelayedFileNum;
    int DelayedLocalEntry;
    int DelayedTrigNum;

    double IbdTimeIntervalMin;//IbdTimeIntervalMin        
    double IbdTimeIntervalMax;//IbdTimeIntervalMax 
    double promptELow;//PromptEnergyLow                
    double promptEHigh;//PromptEnergyHigh                    
    double delayedELow;//DelayedEnergyLow                    
    double delayedEHigh;//DelayedEnergyHigh
	double AdMuonELow;//AdMuonELow
	double AdMuonEHigh;//AdMuonEHigh
    double DelayedTrigTime2AdMuon;//DelayedTrigTime2AdMuon
	double DelayedTrigTime2IWpMuon;//DelayedTrigTime2WpMuon
	double DelayedTrigTime2OWpMuon;//DelayedTrigTime2WpMuon
	double DelayedTrigTime2AdShowerMuon;//DelayedTrigTime2AdShowerMuon
	double Time2LastBufEvent;//Time2LastBufEvent
	double Time2PreAdEvent_l;
	double Time2NextAdEvent_l;
	double DelayedTrigTime2AdMuon_l;
	double DelayedTrigTime2IWpMuon_l;
	double DelayedTrigTime2OWpMuon_l;
	double DelayedTrigTime2AdShowerMuon_l;
};
