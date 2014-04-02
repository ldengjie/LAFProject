#ifndef MICHEALALG_H
#define MICHEALALG_H

#include "LafKernel/AlgBase.h"
#include "LafKernel/PhyEventBuf.h"
#include "TTimeStamp.h"
#include <TNtuple.h>

class MichelAlg : public AlgBase
{
  public:
    MichelAlg(const std::string& name);
    virtual ~MichelAlg(){}

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

  private:

    PhyEventBuf* m_buffer;
    TNtuple *m_tuple;
    double m_muEngLow;
    double m_muEngHigh;
    double m_t2muLow;
    double m_t2muHigh;
    double m_muEng[4];  // muon energy for each AD
    TTimeStamp m_muTime[4]; // muon timestamp for each AD

};

#endif //MICHEALALG_H
