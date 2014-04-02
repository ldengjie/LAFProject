#ifndef ADENERGYALG_H
#define ADENERGYALG_H

#include "LafKernel/AlgBase.h"
#include "LafKernel/PhyEventBuf.h"
#include "TNtuple.h"

class AdEnergyAlg : public AlgBase
{
  public:
    AdEnergyAlg(const std::string& name);
    virtual ~AdEnergyAlg(){}

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

  private:
    PhyEventBuf* m_buffer;
    TNtuple *m_tuple;
};

#endif //ADENERGYALG_H
