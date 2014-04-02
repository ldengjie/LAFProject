#ifndef LIVETIMEALG_H
#define LIVETIMEALG_H

#include "LafKernel/AlgBase.h"
#include "LafKernel/PhyEventBuf.h"
#include <TTimeStamp.h>

class MuonVeto;
class LiveTimeSvc;

class LiveTimeAlg : public AlgBase
{
  public:
    LiveTimeAlg(const std::string& name);
    virtual ~LiveTimeAlg(){}

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

  private :

    PhyEventBuf*  m_buffer;
    LiveTimeSvc *m_liveTimeSvc;
    PhyEvent *m_lastEvt;

    TTimeStamp m_beginTime;
    TTimeStamp m_endTime;
    double m_timeGap;
    
    MuonVeto *m_muonVeto;

    int m_timeBin;
    double m_timeGapInCurBin;

};

#endif //LIVETIMEALG_H
