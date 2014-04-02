#ifndef SINGLESALG_H
#define SINGLESALG_H

#include "LafKernel/AlgBase.h"
#include "LafKernel/PhyEventBuf.h"
#include "LafKernel/PhyEvent/RecReadout.h"
#include "LafKernel/PhyEvent/CalibReadout.h"
#include "LiveTimeSvc/LiveTimeSvc.h"
#include "MuonNeutronBufSvc/MuonNeutronBufSvc.h"

#include "TNtuple.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include "TTimeStamp.h"

#include <map>
#include <deque>
#include <vector>
#include <utility>

#define  ArrayMaxLen 1000
#define  SubArrayLen 10
#define  NumOfHitMax 1000

typedef std::deque<PhyEvent*> Deque;

class SinglesAlg : public AlgBase
{
  public:
    SinglesAlg(const std::string& name);
    virtual ~SinglesAlg(){}

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

  private:
    int          fillSingles(const Deque &evtGroup);
 
  private:
    // Main buffer
    PhyEventBuf* m_buffer;
    //RecReadout* m_adSimple;
    // User buffer
    Deque m_adSingles[4];
    LiveTimeSvc *m_liveTimeSvc;
    MuonNeutronBufSvc *m_muonNeutronBufSvc;

    // Define option variables
    double m_eSinglesHigh; // Set higher limit for single events
    double m_eSinglesLow;  // Set lower limit for single events
    double m_t2preLow; // Set lower limit of delta t to previous event for single events
    double m_t2postLow;// Set lower limit of delta t to next event for single events
    double m_t2MuonHigh; // Set upper limit of time since muon for isotope events
    bool   m_findSingle;

    // Define histograms
    TH1D* m_hMuonSpec[4];
    TH1D* m_hMuonSpecAftRed[4];
    TH2D* m_hMuonEnergyVsNeuMulti[4];
    TH1D* m_hNeutronMultiplicity[4];

    TH1D* m_hSinglesSpec[4];
    TH1D* m_hTime2LastSingle[4];
    TH1D* m_hSinglesRate[4];

    TH2D* m_hSinglesYvsX[4];
    TH2D* m_hSinglesZvsR2[4];

    TH1D* m_hAdTrigSpec_afterMuonVeto[4];
    TH1D* m_hAdTrigSpec_noMuonVeto[4];
    PhyEvent* m_lastSingle[4];

  private:
    // Define tree and branches
    TTree *m_tSingles;
    float m_t2pre;
    float m_t2post;
    float m_ePre;
    float m_ePost;

    int   m_muonMulti;
    float m_t2Muon[ArrayMaxLen];
    float m_eMuon[ArrayMaxLen];
    int   m_nPostNeuHGd[ArrayMaxLen];

    int   m_det;
    //int   m_trigType;
    int   m_trigSec;
    int   m_trigNanoSec;
    //int   m_fileNum;
    //int   m_localEntry;

    //int   m_nHit;
    //float m_hitTime[NumOfHitMax];
    //float m_hitCharge[NumOfHitMax];

    //float m_energy;
    float m_enrec;
    //float m_eprec;
    //float m_adSimpleX;
    //float m_adSimpleY;
    //float m_adSimpleZ;
    float m_x;
    float m_y;
    float m_z;
    float m_xPre;
    float m_yPre;
    float m_zPre;
    float m_xPost;
    float m_yPost;
    float m_zPost;
    float m_quadrant;
    float m_maxQ;
    //float m_maxQ_2inchPMT;

};
#endif //SINGLESALG_H
