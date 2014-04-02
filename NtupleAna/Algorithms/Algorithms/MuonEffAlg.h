#ifndef MUONEFFALG_H
#define MUONEFFALG_H

#include "LafKernel/AlgBase.h"
#include "LafKernel/PhyEventBuf.h"
#include <TNtuple.h>

class MuonEffAlg : public AlgBase
{
  public:
    MuonEffAlg(const std::string& name);
    virtual ~MuonEffAlg(){}

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

  private:

    PhyEventBuf *m_buffer;
    TNtuple     *m_tuple;

    double m_search_twd;
    int    m_eBinNum;
    double m_eStep;
    double m_energyCut[100];
    //double m_ad1iws_eff[4][100];
    //double m_ad1iws_eff_err[4][100];
    //double m_ad1ows_eff[4][100];
    //double m_ad1ows_eff_err[4][100];
    unsigned int m_adNum[4][100];
    unsigned int m_iwsNum[4][100];
    unsigned int m_owsNum[4][100];
};

#endif //MUONEFFALG_H
