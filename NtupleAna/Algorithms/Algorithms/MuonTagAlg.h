#ifndef MUONTAGALG_H
#define MUONTAGALG_H

#include "LafKernel/AlgBase.h"
#include "LafKernel/PhyEventBuf.h"
#include <TTimeStamp.h>

class MuonVeto;
class TH1F;
class TH2F;
class LiveTimeSvc;

class MuonTagAlg : public AlgBase
{
  public:
    MuonTagAlg(const std::string& name);
    virtual ~MuonTagAlg(){}

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

  private :

    PhyEventBuf*  m_buffer;

    TTimeStamp m_beginTime;
    TTimeStamp m_endTime;
    
    MuonVeto *m_muonVeto;
    LiveTimeSvc *m_liveTimeSvc;

  private:
    void doNewBin(int bin);
    int m_hIwsMuonNumber;
    int m_hOwsMuonNumber;
    int m_hRpcMuonNumber;
    int m_hIsoRpcMuonNumber;
    int m_hAdMuonNumber[4];
    int m_hShowerMuonNumber[4];
    int m_hAdMuonNewNumber[4];
    int m_hShowerMuonNewNumber[4];

    TH1F* m_hIwsMuonNum;
    TH1F* m_hOwsMuonNum;
    TH1F* m_hRpcMuonNum;
    TH1F* m_hIsoRpcMuonNum;
    TH1F* m_hAdMuonNum[4];
    TH1F* m_hShowerMuonNum[4];
    TH1F* m_hAdMuonNewNum[4];
    TH1F* m_hShowerMuonNewNum[4];

    TH1F* m_hIwsMuon;
    TH1F* m_hOwsMuon;
    TH1F* m_hRpcMuon;
    TH1F* m_hAdMuon[4];
    TH1F* m_hShowerMuon[4];
    TH1F* m_hAdMuonNew[4];
    TH1F* m_hShowerMuonNew[4];

    int m_timeBin;
    double m_vetoTimeInCurBin[4];
    double m_vetoTimeInCurBinOld[4];
    double m_vetoTimeAccum[4];  // Accumulated muon veto time in previous time bins
    double m_vetoTimeAccumOld[4];  // Accumulated muon veto time in previous time bins
    double m_SingleLiveTimeAccum[4];
    double m_SingleLiveTimeAccumOld[4];
    double m_SingleLiveTimeAccum200[4];
    double m_SingleLiveTimeAccum200Old[4];

    double m_vetoTimeInCurBin4IbdSel[4];
    double m_vetoTimeInCurBin4IbdSelOld[4];
    double m_vetoTimeAccum4IbdSel[4];  // Accumulated muon veto time in previous time bins
    double m_vetoTimeAccum4IbdSelOld[4];  // Accumulated muon veto time in previous time bins

};

#endif //MUONTAGALG_H
