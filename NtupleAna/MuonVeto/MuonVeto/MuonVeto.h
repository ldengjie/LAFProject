#ifndef MUONVETO_H
#define MUONVETO_H

#include "LafKernel/PhyEventBuf.h"
#include "TTimeStamp.h"
#include "LiveTimeSvc/LiveTimeSvc.h"

class PhyEvent;

class MuonVeto
{
  public:
    MuonVeto();
    virtual ~MuonVeto(){}
    static MuonVeto* instance();

    LiveTimeSvc* m_liveTimeSvc;
    void updateVetoWindow(PhyEvent *event);
    int veto(PhyEvent *event);
    PhyEvent* preMuon(PhyEvent *event, const int det=0);
    PhyEvent* nextMuon(PhyEvent *event, const int det=0);

    PhyEvent* preAdMuon(PhyEvent *event, double eLow, double eHigh);
    PhyEvent* preShowerMuon(PhyEvent *event);
    PhyEvent* preAdMuonNew(PhyEvent *event, double eLow, double eHigh);
    PhyEvent* preShowerMuonNew(PhyEvent *event);

    double muVetoWin() { return m_muVetoWin; }
    double adMuVetoWin() { return m_adMuVetoWin; }
    double showerMuVetoWin() { return m_showerMuVetoWin; }
    double preVetoWin() { return m_preVetoWin; }

    TTimeStamp m_vetoWindowPost[4];  // post timestamp of muon veto window for 4 ADs
    TTimeStamp m_vetoWindowPostOld[4];  // post timestamp of muon veto window for 4 ADs
    double m_deadTime[4];
    double m_deadTimeOld[4];
    TTimeStamp m_vetoWindowPost4IbdSel[4];  // post timestamp of muon veto window for 4 ADs
    TTimeStamp m_vetoWindowPost4IbdSelOld[4];  // post timestamp of muon veto window for 4 ADs
    double m_deadTime4IbdSel[4];
    double m_deadTime4IbdSelOld[4];
    //double vetoWindowPost_temp[4];
    //double m_deadTime_temp[4];
    double m_SingleLiveTime[4];
    double m_SingleLiveTimeOld[4];
    double m_SingleLiveTime200[4];
    double m_SingleLiveTime200Old[4];

    const std::string& name() { return m_name; }

  private:
    const TTimeStamp timeCnv(double time);
    //DataBuffer<PhyEvent>::Iterator iter(PhyEvent *event);

  private:

    PhyEventBuf*    m_buffer;
    std::string     m_name;

    static MuonVeto* m_instance;

  private:

    double m_muVetoWin;
    double m_adMuVetoWin;
    double m_showerMuVetoWin;
    double m_muVetoWin4IbdSel;
    double m_adMuVetoWin4IbdSel;
    double m_showerMuVetoWin4IbdSel;
    double m_preVetoWin;
    bool   m_useRPCVeto;
};

#endif //MUONVETO_H
