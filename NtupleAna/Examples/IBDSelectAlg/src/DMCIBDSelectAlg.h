#ifndef DMCIBDSELECTALG_H
#define DMCIBDSELECTALG_H

#include "LafKernel/AlgBase.h"
#include "LafKernel/PhyEventBuf.h"

#include <vector>

class TTree;

class DMCIBDSelectAlg : public AlgBase
{
  public:
    DMCIBDSelectAlg(const std::string& name);
    virtual ~DMCIBDSelectAlg(){}

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

  private:
    bool fillIbd(const std::vector<PhyEvent*> &evtGroup);

  private:
    PhyEventBuf* m_buffer;
    TTree *m_tree;

    // For IBD tree
    double m_tep;
    double m_txp;
    double m_typ;
    double m_tzp;
    double m_ten;
    double m_txn;
    double m_tyn;
    double m_tzn;
    double m_tdt;
    int m_tdet;
    int m_tpfile;
    int m_tpentry;
    int m_tnfile;
    int m_tnentry;
    int m_tptrignum;
    int m_tntrignum;
    int m_tn;

    std::vector<PhyEvent*> m_adBuf[4];

  private:
    double m_eplow;
    double m_ephigh;
    double m_edlow;
    double m_edhigh;
    double m_dtlow;
    double m_dthigh;
};

#endif //DMCIBDSELECTALG_H
