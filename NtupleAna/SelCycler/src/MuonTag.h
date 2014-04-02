#ifndef MUONTAG_H
#define MUONTAG_H

#include "SelCycler/ITagBase.h"

class MuonTag : public ITagBase
{
  public:
    MuonTag(const std::string& name);
    virtual ~MuonTag(){}
    virtual bool tag(PhyEvent* event);

  private:
    double m_adMuonThr;
    double m_showerMuonThr;
    int m_iwsNpmtThr;
    int m_owsNpmtThr;
    bool m_tagRPCMuon;
    bool m_tagADMuon;
};

#endif //MUONTAG_H
