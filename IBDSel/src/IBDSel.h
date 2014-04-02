#include  <iostream>
#include  "TH1F.h"
#include  "TTree.h"
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
#include  "LiveTimeSvc/LiveTimeSvc.h"
#include    "LafKernel/PhyEvent/CalibReadout.h"
using namespace std;  

class IBDSel : public AlgBase
{
  public:
	IBDSel(const std::string& name);
	virtual ~IBDSel(){}

	virtual bool initialize();
	virtual bool execute();
	virtual bool finalize();
  private:
	bool FillIbd(vector<PhyEvent*> EvtGroup,int tag);
	bool FillSingle(PhyEvent* Evt,int tag);
	//bool FillMulti(vector<PhyEvent*> EvtGroup);
	bool CalTime2Muon(PhyEvent* event);
	bool printEvt(PhyEvent* CurEvent);
    void dump(int i_det,bool IsFinal,PhyEvent* CurEvent);

    int ADMuonNum[7];
	bool VetoedByRpc;
	int IsIbd;
	double time2Muon[22];
	TString histName;
	//int Ebins;
	int det;
    double trigTime;
    long entry_l;
    int fileNum_l;
    long localEntry;
    int trigNum_l;
	double t2muon[22];
	
	bool saveSingle;
	TTree* SingleTree;
	float x;
	float y;
	float z;
	float energy;
	int IsSingle;
	int IsEle;
	int IsEle_600;
	int IsNeu;
	double curLivetime;
	double curLivetimeOld;
	double curDaqtime;
	double curDaqtimeOld;

	bool saveMuon;
	double relatedTime;
	TTree* MuonTree;
	int muonTag;
    int trigType;
    int nPmt;

	TTree* IbdTree;

	//TTree* SingleTree;
	//TTree* MutilTree;
	//[0]for IBD Prompt singal,[1]for IBD delayed singal
	int isIbd_l;
	int    det_l;
	float  energy_l[2];
	float  x_l[2];
	float  y_l[2];
	float  z_l[2];
	int fileNum[2];
	long entry[2];
	int trigNum[2]; 
	//double time2Muon[2];
	double timeInterval; //us
	int delayedTrigTimeSec;
	double promptT2Muon[22];
	double delayedT2Muon[22];

    //int num_temp;
    //int num_total;
    //int num_temp_N0;
    //int num_temp_2;
    int IsLi;
    int isLi_l;
	double DelayedTrigTime2AdMuon4Li;
	double DelayedTrigTime2IWpMuon4Li;
	double DelayedTrigTime2OWpMuon4Li;
	double DelayedTrigTime2RpcMuon4Li;
	double DelayedTrigTime2AdShowerMuon4Li;

    TTimeStamp lastOwpMuonTrigtime;
    TTimeStamp lastIwpMuonTrigtime;
    TTimeStamp lastRpcMuonTrigtime;
    TTimeStamp lastAdMuonTrigtime[4];
    TTimeStamp lastShowerMuonTrigtime[4];
    TTimeStamp lastNewAdMuonTrigtime[4];
    TTimeStamp lastNewShowerMuonTrigtime[4];

	TTimeStamp lastshowermuonTrigtime[6][4];
	TTimeStamp lastshowermuonTrigtime0[6][4];
	vector<TTimeStamp> lastshowermuonTrigtimeVec[6][4];
	vector<TTimeStamp> lastshowermuonTrigtime0Vec[6][4];
	//double lastshowermuonTrigtime[6][4];
	//double lastshowermuonTrigtime0[6][4];
	//vector<double> lastshowermuonTrigtimeVec[6][4];
	//vector<double> lastshowermuonTrigtime0Vec[6][4];
	TH1F* time2lastshowermuon[6];
	TH1F* time2lastshowermuon0[6];
	TH1F* showermuonNum[6];
	TH1F* showermuonNum0[6];
/*
*/
	TH1F* NumN[4];
	TH1F* NumE[4];
	TH1F* NumE_600[4];
	TH1F* NumN0[4];
	TH1F* NumE0[4];
	TH1F* NumE0_600[4];
	TH1F* NumOfAll[4];
	TH1F* NumOfAll0[4];

    /*
    void doNewBin(int bin);
    int m_timeBin;
	int NumNNumber[4];
	int NumENumber[4];
	int NumE_600Number[4];
	int NumN0Number[4];
	int NumE0Number[4];
	int NumE0_600Number[4];
	int NumOfAllNumber[4];
	int NumOfAll0Number[4];
*/
	//TH1F* RpcMuonRate;
	//TH1F* OWpMuonRate;
	//TH1F* IWpMuonRate;
	//TH1F* AdMuonRate[4];
	//TH1F* AdShowerMuonRate[4];
	//TH1F* IbdRate[4];
	//TH1F* IbdPromptEnergy[4];
	//TH1F* IbdDelayedEnergy[4];
	//TH1F* IbdDistance[4];//>1.8M?
	//TH1F* IbdTimeInterval[4];

	LiveTimeSvc* liveTimeSvc;
	PhyEventBuf* EvtBuffer;
	PhyEvent* CurEvent;
	vector<PhyEvent*> AdEvtBuf[4];
	vector<bool> VetoedBuf[4];
    vector<double> time2MuonVec;
    vector< vector<double> > time2MuonBuf[4];
	MuonVeto* muonVeto_l;
	
	double promptELow;//PromptEnergyLow
	double promptEHigh;//PromptEnergyHigh
	double promptEHigh4Fn;//PromptEnergyHigh4Fn
	double delayedEnergyLow4Li;//PromptEnergyLow
	double delayedEnergyHigh4Li;//PromptEnergyHigh
	double delayedELow;//DelayedEnergyLow
	double delayedEHigh;//DelayedEnergyHigh
	double AdMuonELow;//AdMuonELow
	double AdMuonEHigh;//AdMuonEHigh

	double IbdTimeIntervalMin;//IbdTimeIntervalMin
	double IbdTimeIntervalMax;//IbdTimeIntervalMax
	double IbdDisInterval;//IbdTimeIntervalMax
	double Time2LastBufEvent;//Time2LastBufEvent
	//double Time2PreAdEvent;//Time2PreAdEvent
	//double Time2NextAdEvent;//Time2NextAdEvent
	//double Time2PreAdEvent_l;
	//double Time2NextAdEvent_l;
	double DelayedTrigTime2AdMuon;//DelayedTrigTime2AdMuon
	double DelayedTrigTime2IWpMuon;//DelayedTrigTime2IWpMuon
	double DelayedTrigTime2OWpMuon;//DelayedTrigTime2OWpMuon
	double DelayedTrigTime2RpcMuon;//DelayedTrigTime2RpcMuon
	double DelayedTrigTime2AdShowerMuon;//DelayedTrigTime2AdShowerMuon
	double PromptTrigTime2AdShowerMuon;//PromptTrigTime2AdShowerMuon
};
