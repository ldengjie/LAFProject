#ifndef IBDSELSIMPALG_H
#define IBDSELSIMPALG_H

#include "LafKernel/AlgBase.h"
#include "LafKernel/PhyEventBuf.h"
#include "TTree.h"
#include "MuonVeto/MuonVeto.h"
#include "TH1D.h"
#include "TH2D.h"

#include <vector>
#include <list>

#define MaxNevt 200
#define NpreMus 600

class IBDSelSimpAlg : public AlgBase
{
  public:
    IBDSelSimpAlg(const std::string& name);
    virtual ~IBDSelSimpAlg(){}

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

  private:
    int PromptFileNum;
	int PromptLocalEntry;
    int PromptTrigNum;
	int DelayedFileNum;
	int DelayedLocalEntry;
	int DelayedTrigNum;
    const TTimeStamp timeCnv(double time);
    //void updateVetoWindow(PhyEvent *event);
    //void updateDaqLiveTime();

    PhyEvent* preShowerMuon(PhyEvent *event, const int det);
    PhyEvent* preADEvt(PhyEvent *event, const int det);
    PhyEvent* nextADEvt(PhyEvent *event, const int det);
    //bool findCloseNonADMuon(PhyEvent* event, double preWin, double nextWin);

    bool calcT2Mus(PhyEvent* event);
    void processEvt(PhyEvent* event, std::vector<PhyEvent*>& evtBuf);
    bool fillCoinc(const std::vector<PhyEvent*> &evtGroup, bool IsFind);

    void processNeuEvt(PhyEvent* event, std::vector<PhyEvent*>& evtBuf);
    bool fillNeu(const std::vector<PhyEvent*> &evtGroup);

    void drawMuons(PhyEvent* event);
    void drawTriggersCutPoolMuon(PhyEvent* event);
    void drawTriggersCutShowerMuon(PhyEvent* event);

    bool dump(const std::vector<PhyEvent*> &evtGroup);

    void print(PhyEvent* event);
    bool drawSingles(PhyEvent* event);
  private:
    PhyEventBuf* m_buffer;
    PhyEventBuf* m_mainBuf;
    TTree *m_ibdTree;
    TTree *m_swapIbdTree;
    TTree *m_neuTree;

    std::vector<PhyEvent*> m_adBuf[4];
    std::vector<PhyEvent*> m_adVetoedBuf[4];
    std::list<PhyEvent*> m_adMuonBuf[4];
    std::vector<PhyEvent*> m_neuBuf[4];
    std::vector<PhyEvent*> m_neuVetoedBuf[4];

  private:
    double m_eplow;
    double m_ephigh;
    double m_edlow;
    double m_edhigh;
    double m_dtlow;
    double m_dthigh;
    bool   m_drawHists;
    bool   m_savePreAdMus;
    bool   m_useRPCVeto;
    double m_adMuonThr;
    double m_showerMuonThr;
    bool   m_tagRPCMuon;
    bool   m_tagADMuon;
    double m_adMuonBufThr;

    double* eBinWt;
    TTimeStamp m_beginTime;
    TTimeStamp m_endTime;
    double m_muVetoWin;
    double m_adMuVetoWin;
    double m_showerMuVetoWin;
    double m_preVetoWin;
    double m_t2Mus[9];
    int    m_lastFileNum;
    // index 0, time to previous IWS muon
    // index 1, time to next IWS muon
    // index 2, time to previous OWS muon
    // index 3, time to next OWS muon
    // index 4, time to previous RPC muon
    // index 5, time to next RPC muon
    // index 6, time to AD non-shower muon
    // index 7, time to AD shower muon

    int       m_isVetoed;
    int       m_isIbd;
    MuonVeto* m_muonVeto;
    TTimeStamp m_vetoWindowPost[2][4];  
    double m_deadTime[2][4];
    // post timestamp of muon veto window for 4 ADs
    // Record non-shower muon veto and shower muon veto, respectively

    int       m_det;
    int       m_Nevt;
    int       m_fstTrigSec;
    int       m_fstTrigNanoSec;
    double    m_t2preADEvt;
    double    m_t2nextADEvt;
    float     m_preADEvtE;
    float     m_nextADEvtE;
    int       m_TrigType[MaxNevt];
    float     m_Energy[MaxNevt];
    float     m_t2preRPCMuon[MaxNevt];
    float     m_t2preIWSMuon[MaxNevt];
    float     m_t2preOWSMuon[MaxNevt];
    float     m_t2nextIWSMuon[MaxNevt];
    float     m_t2nextOWSMuon[MaxNevt];
    float     m_t2nextRPCMuon[MaxNevt];
    float     m_t2nonShowerMuon[MaxNevt];
    float     m_t2showerMuon[MaxNevt];
    float     m_t2nextNonShowerMuon[MaxNevt];
    float     m_preNonShowerMuonE[MaxNevt];
    float     m_preShowerMuonE[MaxNevt];
    float     m_X[MaxNevt];
    float     m_Y[MaxNevt];
    float     m_Z[MaxNevt];
    float     m_MaxQ[MaxNevt];
    float     m_Quadrant[MaxNevt];
    float     m_middleTimeRMS[MaxNevt];
	  float     m_maxQ_2inchPMT[MaxNevt];
	  float     m_Column_2inchPMT[MaxNevt];
    double    m_deltaT[MaxNevt];
    int       m_fileSeqNum[MaxNevt];
    int       m_lclRoNum[MaxNevt];
    int       m_nPmt[MaxNevt];
    float     m_t2preAdMus[NpreMus];
    float     m_preAdMusE[NpreMus];

    float     m_rawEvis[MaxNevt]; 
    float     m_enrec[MaxNevt];  
    float     m_eprec[MaxNevt];  
    float     m_scaledE[MaxNevt]; 

    // Histograms
    TH1D*     triggerSpecAll[4];
    TH1D*     triggerSpecAllLog[4];
    TH1D*     triggerSpecAllAfterPoolMuonVeto[4];
    TH1D*     triggerSpecAllAfterPoolMuonVetoLog[4];
    TH1D*     triggerSpecAllAfterShowerMuonVeto[4];
    TH1D*     triggerSpecAllAfterShowerMuonVetoLog[4];
    TH1D*     triggerSpecAllCutFlasher[4];
    TH1D*     triggerSpecAllCutFlasherLog[4];

    TH1D*     lowESinglesSpecAfterPoolMuonVeto[4];
    TH1D*     lowESinglesRateAfterPoolMuonVeto[4];
    TH1D*     neuSinglesSpecAfterPoolMuonVeto[4];
    TH1D*     neuSinglesRateAfterPoolMuonVeto[4];
    TH1D*     highESinglesSpecAfterPoolMuonVeto[4];
    TH1D*     highESinglesRateAfterPoolMuonVeto[4];

    TH1D*     lowESinglesSpecAfterShowerMuonVeto[4];
    TH1D*     lowESinglesRateAfterShowerMuonVeto[4];
    TH1D*     neuSinglesSpecAfterShowerMuonVeto[4];
    TH1D*     neuSinglesRateAfterShowerMuonVeto[4];
    TH1D*     highESingleSpecAfterShowerMuonVeto[4];
    TH1D*     highESinglesRateAfterShowerMuonVeto[4];
    TH1D*     highESinglesSpecAfterShowerMuonVeto[4];

    TH1D*     hNevt[4];
    TH1D*     muonSpec[4];

    TH1D*     triggerRateLowAll[4];
    TH1D*     triggerRateLowAfterPoolMuonVeto[4];
    TH1D*     triggerRateLowAfterShowerMuonVeto[4];
    TH1D*     triggerRateLowCutFlasher[4];

    TH1D*     triggerRateHighAll[4];
    TH1D*     triggerRateHighAfterPoolMuonVeto[4];
    TH1D*     triggerRateHighAfterShowerMuonVeto[4];
    TH1D*     triggerRateHighCutFlasher[4];

    TH1D*     triggerRateAll[4];
    TH1D*     triggerRateAllAfterPoolMuonVeto[4];
    TH1D*     triggerRateAllAfterShowerMuonVeto[4];
    TH1D*     triggerRateAllCutFlasher[4];

    TH1D*     triggerRate[7][4];
    TH1D*     ibdRate[4];
    TH1D*     ibdPosE[4];
    TH1D*     ibdNeuE[4];
    TH1D*     ibdTime[4];

    TH1D*     adMuonRate[4];
    TH1D*     showerMuonRate[4];
    TH1D*     rpcMuonRate;
    TH1D*     IWSMuonRate;
    TH1D*     OWSMuonRate;
    TH1D*     flasherRate[4][4];

    TH2D*     coinc2d[4];
};

#endif //IBDSELSIMPALG_H
