#ifndef IBDSELECTALG_H
#define IBDSELECTALG_H

#include "LafKernel/AlgBase.h"
#include "LafKernel/PhyEventBuf.h"

#include <vector>

class TTree;
class TH1F;
class MuonVeto;
class LiveTimeSvc;

class IBDSelectAlg : public AlgBase
{
  public:
    IBDSelectAlg(const std::string& name);
    virtual ~IBDSelectAlg(){}

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

  private:
    bool fillIbd(const std::vector<PhyEvent*> &evtGroup);
    bool fillSingles(const std::vector<PhyEvent*> &evtGroup);
    bool isVetoedByWp(PhyEvent *event);

  private:
    
    //Services
    PhyEventBuf* m_buffer;
    LiveTimeSvc *m_liveTimeSvc;
    MuonVeto *m_muonVeto;

    // Buffer of event segment for each AD
    std::vector<PhyEvent*> m_adBuf[4];

    // For IBD tree
    TTree *m_tree;
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
    int m_tn; // trigger of delayed

    // Histogram
    TH1F* m_ibdNum[4]; // IBD candidate rate VS time
    TH1F *m_hPSingles[4];
    TH1F *m_hNSingles[4];

  private:  // options
    bool m_saveTree;
    double m_eplow;
    double m_ephigh;
    double m_edlow;
    double m_edhigh;
    double m_dtlow;
    double m_dthigh;

  private:  // statistics
    long int  m_nibd[4]; // number of IBD candidates
    long int  m_npsingles[4];  // number of prompt-like singles
    long int  m_nnsingles[4];  // number of neutron-like singles
};

#endif //IBDSELECTALG_H
