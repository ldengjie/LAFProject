#ifndef OFFWINDOWALG_H
#define OFFWINDOWALG_H

#include "LafKernel/AlgBase.h"
#include "LafKernel/PhyEventBuf.h"

#include <vector>

class TTree;
class TH1F;
class MuonVeto;
class LiveTimeSvc;

class OffWindowAlg : public AlgBase
{
  public:
    OffWindowAlg(const std::string& name);
    virtual ~OffWindowAlg(){}

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

  private:
    bool isVetoedByWp(PhyEvent *event);
    bool fillOffWinAcc(PhyEvent *event);

  private:
    
    //Services
    PhyEventBuf* m_buffer;
    LiveTimeSvc *m_liveTimeSvc;
    MuonVeto *m_muonVeto;

    // Buffer of event segment for each AD
    std::vector<PhyEvent*> m_adSingleBuf[4];

    // For IBD tree
    TTree *m_tree;
    int m_offwMethod; // method 1 or 2, see doc:7622
    int m_ioffw;  // the ith off-window
    double m_tep; // energy of prompt
    double m_txp; // x-position of prompt
    double m_typ; // y-position of prompt
    double m_tzp; // z-position of prompt
    double m_ten; // energy of delayed
    double m_txn; // x-position of delayed
    double m_tyn; // y-position of delayed
    double m_tzn; // z-position of delayed
    double m_tdt; // time interval of prompt and delayed
    int m_tdet;   // detector
    int m_tpfile; // file number of prompt
    int m_tpentry; // local entry of prompt
    int m_tnfile;  // file number of delayed
    int m_tnentry; // local entry of delayed
    int m_tptrignum; // trigger number of prompt
    int m_tntrignum; // trigger number of delayed
    int m_tn; // trigger time of delayed

  private:
    TH1F *m_hNSingles[4]; // used for correction in methed 1, see doc:7622
    TH1F *m_hNCorr[4]; // used for correction in methed 1, see doc:7622

  private:  // options
    double m_eplow;
    double m_ephigh;
    double m_edlow;
    double m_edhigh;
    double m_dtlow;
    double m_dthigh;
    double m_offWindow;
    int m_offWindowBin;

};

#endif //OFFWINDOWALG_H
