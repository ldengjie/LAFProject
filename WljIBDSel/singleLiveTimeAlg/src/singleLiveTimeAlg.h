#ifndef SINGLELIVETIMEALG_H
#define SINGLELIVETIMEALG_H

#include "LafKernel/AlgBase.h"
#include "LafKernel/PhyEventBuf.h"
#include "TTree.h"
#include "MuonVeto/MuonVeto.h"
#include "TH1D.h"

#include <vector>
#include <list>

class singleLiveTimeAlg : public AlgBase
{
  public:
    singleLiveTimeAlg(const std::string& name);
    virtual ~singleLiveTimeAlg(){}

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

  private:
    const TTimeStamp timeCnv(double time);
    void updateVetoWindow(PhyEvent *event);
    void updateDaqLiveTime();

  private:
    PhyEventBuf* m_buffer;

    bool   m_useRPCVeto;
    //double m_adMuonThr;
    //double m_showerMuonThr;
    //bool   m_tagRPCMuon;
    //bool   m_tagADMuon;
    //int    m_iwsNpmtThr;
    //int    m_owsNpmtThr;

    TTimeStamp m_beginTime;
    TTimeStamp m_endTime;
    double m_muVetoWin;
    double m_adMuVetoWin;
    double m_showerMuVetoWin;
    double m_preVetoWin;
    int    m_lastFileNum;

    MuonVeto* m_muonVeto;
    TTimeStamp m_vetoWindowPost[2][4];  
    double m_deadTime[2][4];
    // post timestamp of muon veto window for 4 ADs
    // Record non-shower muon veto and shower muon veto, respectively

    // Histograms
    TH1D*     vetoTimeForTrigPlots[4];
    TH1D*     vetoTimeForIBDSel[4];
    TH1D*     gapTime[4];
    TH1D*     daqLiveTime;
    TTree*    m_tree;
    int       m_det;
    int       m_timeBin;
    int       m_liveWindowSec;
    int       m_liveWindowNanoSec;
};

#endif //SINGLELIVETIMEALG_H
