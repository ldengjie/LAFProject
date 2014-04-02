#include "Algorithms/LiveTimeAlg.h"
#include "MuonVeto/MuonVeto.h"
#include "LafKernel/GlobalVar.h"
#include "LiveTimeSvc/LiveTimeSvc.h"
#include "TTimeStamp.h"
#include "TH1F.h"
#include "TTree.h"
#include <iostream>

using namespace std;

  LiveTimeAlg::LiveTimeAlg(const std::string& name)
: AlgBase(name),m_lastEvt(0),m_timeGap(0.),m_timeBin(-1)
{
}

bool LiveTimeAlg::initialize()
{
  m_buffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
  m_liveTimeSvc = dynamic_cast<LiveTimeSvc*>(service("LiveTimeSvc"));
  if(!m_liveTimeSvc) {
    LogError << "Can't get LiveTimeSvc." << endl;
    return false;
  }
  m_liveTimeSvc->initHist();
  m_muonVeto = MuonVeto::instance();

  ntupleSvc()->attach("FILE1/LiveTime/",m_liveTimeSvc->hDaqTime());
  //ntupleSvc()->attach("FILE1/LiveTime/",m_liveTimeSvc->tSingleLiveTimeWindow());
  //ntupleSvc()->attach("FILE1/LiveTime/",m_liveTimeSvc->tSingleLiveTimeWindowOld());
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    ntupleSvc()->attach("FILE1/LiveTime/",m_liveTimeSvc->hLiveTime(det));
    ntupleSvc()->attach("FILE1/LiveTime/",m_liveTimeSvc->hLiveTimeOld(det));
    ntupleSvc()->attach("FILE1/LiveTime/",m_liveTimeSvc->hLiveTime4IbdSel(det));
    ntupleSvc()->attach("FILE1/LiveTime/",m_liveTimeSvc->hLiveTime4IbdSelOld(det));
    ntupleSvc()->attach("FILE1/LiveTime/",m_liveTimeSvc->hSingleLiveTime(det));
    ntupleSvc()->attach("FILE1/LiveTime/",m_liveTimeSvc->hSingleLiveTimeOld(det));
    ntupleSvc()->attach("FILE1/LiveTime/",m_liveTimeSvc->hSingleLiveTime200(det));
    ntupleSvc()->attach("FILE1/LiveTime/",m_liveTimeSvc->hSingleLiveTime200Old(det));
    ntupleSvc()->attach("FILE1/LiveTime/",m_liveTimeSvc->hNumOfSingleLiveTime200(det));
    ntupleSvc()->attach("FILE1/LiveTime/",m_liveTimeSvc->hNumOfSingleLiveTime200Old(det));

  }

  return true;
}

bool LiveTimeAlg::finalize()
{
  // set daq time for the last time bin
  m_liveTimeSvc->setDaqTimePerBin(m_timeBin,m_endTime-m_liveTimeSvc->startTime(m_timeBin)-m_timeGapInCurBin);
    for( int i=1 ; i<5 ; i++ )
    {
        m_liveTimeSvc->setNumOfSingleLiveTime200(i,m_timeBin);
        m_liveTimeSvc->setNumOfSingleLiveTime200Old(i,m_timeBin);
    }

  // Fill DAQ time histogram
  //for(int i=0;i<m_liveTimeSvc->nBins();i++) {
  //  m_liveTimeSvc->hDaqTime()->Fill(m_liveTimeSvc->startTime(i) + m_liveTimeSvc->binWidth()/2,
  //      m_liveTimeSvc->daqTimePerBin(i));
  //}
  return true;
}

bool LiveTimeAlg::execute()
{
  PhyEvent *event = m_buffer->curEvt();
  /*
    cout << "------entry=" << event->m_entry
      << ", fileNum=" << event->m_fileNum
      << ", local entry=" << event->m_localEntry
      << ", trigtime=" << event->m_trigTime
      << ", det=" << event->m_det;
        if( event->m_det<5 )
        {
      if( m_muonVeto->veto(event) )
      {
         cout<<" vetoed!" ;
      }else
      {
        cout<<"good";
      }
        }
        
      cout<< endl;
    */
    //std::cout<<"FirstGapEventTrigtime  : "<<setprecision(20)<<m_liveTimeSvc->getFirstGapEventTrigtime()<<endl;
    //std::cout<<"LastGapEventTrigtime  : "<<m_liveTimeSvc->getLastGapEventTrigtime()<<endl;
    //std::cout<<"event->m_trigTime  : "<<event->m_trigTime<<endl;
    //std::cout<<setprecision(20)<<"(double)event->m_trigTime  : "<<(double)event->m_trigTime<<endl;
  if(!m_lastEvt) {  // This is the first event
    m_lastEvt = event->GrabInstance();
    m_beginTime = event->m_trigTime;
    m_timeBin = m_liveTimeSvc->bin(event->m_trigTime);
    m_timeGapInCurBin = m_beginTime - m_liveTimeSvc->startTime(m_timeBin);
    m_liveTimeSvc->setFirstGapEventTrigtime(event->m_trigTime);
    m_liveTimeSvc->setLastGapEventTrigtime(event->m_trigTime);
    std::cout<<"This is the first event "<<endl;
    std::cout<<"FirstGapEventTrigtime  : "<<setprecision(20)<<m_liveTimeSvc->getFirstGapEventTrigtime()<<endl;
    std::cout<<"LastGapEventTrigtime  : "<<m_liveTimeSvc->getLastGapEventTrigtime()<<endl;
    
    for( int i=1 ; i<5 ; i++ )
    {
        m_liveTimeSvc->setNumberOfSingleLiveTime200(i,0);
        m_liveTimeSvc->setNumberOfSingleLiveTime200Old(i,0);
    }
    
    //std::cout<<"1  FirstGapEventTrigtime  : "<<setprecision(20)<<m_liveTimeSvc->getFirstGapEventTrigtime()<<endl;
    //std::cout<<"1  LastGapEventTrigtime  : "<<m_liveTimeSvc->getLastGapEventTrigtime()<<endl;
    return true;
  } else {
    m_endTime = event->m_trigTime;
  }
  //std::cout<<"event->m_trigTime-m_lastEvt->m_trigTime  : "<<event->m_trigTime-m_lastEvt->m_trigTime<<endl;

  // caculate daq time in each time bin
  int bin = m_liveTimeSvc->bin(event->m_trigTime);
  double binWidth = m_liveTimeSvc->binWidth();
  if(bin > m_timeBin) { // a new time bin
    double timeGap = m_liveTimeSvc->endTime(m_timeBin) - m_lastEvt->m_trigTime;
    m_liveTimeSvc->setFirstGapEventTrigtime(m_lastEvt->m_trigTime);
    m_liveTimeSvc->setLastGapEventTrigtime(m_liveTimeSvc->endTime(m_timeBin).AsDouble());
    std::cout<<"a new time bin "<<endl;
    std::cout<<"FirstGapEventTrigtime  : "<<m_liveTimeSvc->getFirstGapEventTrigtime()<<endl;
    std::cout<<"LastGapEventTrigtime  : "<<m_liveTimeSvc->getLastGapEventTrigtime()<<endl;
    m_timeGapInCurBin += timeGap;
    m_liveTimeSvc->setDaqTimePerBin(m_timeBin,binWidth-m_timeGapInCurBin);
    LogDebug << "bin:" << bin
      << ", m_timeBin: " << m_timeBin
      << ", m_timeGapInCurBin: " << m_timeGapInCurBin
      << endl;
    m_timeGapInCurBin = 0.;
    for( int i=1 ; i<5 ; i++ )
    {
        m_liveTimeSvc->setNumOfSingleLiveTime200(i,m_timeBin);
        m_liveTimeSvc->setNumOfSingleLiveTime200Old(i,m_timeBin);
        m_liveTimeSvc->setNumberOfSingleLiveTime200(i,0);
        m_liveTimeSvc->setNumberOfSingleLiveTime200Old(i,0);
    }
    
    m_timeBin = bin;
  }

  double dt = event->m_trigTime - m_lastEvt->m_trigTime;
  LogDebug << "Time interval: " << dt << " second." << endl;
  if(dt < 0) {
    LogWarn << "Find wrong event time order." << endl;
    cout << "Current event: " << endl;
    cout << "------entry=" << event->m_entry
      << ", fileNum=" << event->m_fileNum
      << ", local entry=" << event->m_localEntry
      << ", trigtime=" << event->m_trigTime
      << ", det=" << event->m_det
      << endl;
    cout << "Previous event: " << endl;
    cout << "------entry=" << m_lastEvt->m_entry
      << ", fileNum=" << m_lastEvt->m_fileNum
      << ", local entry=" << m_lastEvt->m_localEntry
      << ", trigtime=" << m_lastEvt->m_trigTime
      << ", det=" << m_lastEvt->m_det
      << endl;
  }

  if(dt > m_liveTimeSvc->evtTimeGap()) {
    LogWarn << "Find " << dt << " second event time gap." << endl;
    cout << "Current event: " << endl;
    cout << "------entry=" << event->m_entry
      << ", fileNum=" << event->m_fileNum
      << ", local entry=" << event->m_localEntry
      << ", trigtime=" << event->m_trigTime
      << ", det=" << event->m_det
      << endl;
    cout << "Previous event: " << endl;
    cout << "------entry=" << m_lastEvt->m_entry
      << ", fileNum=" << m_lastEvt->m_fileNum
      << ", local entry=" << m_lastEvt->m_localEntry
      << ", trigtime=" << m_lastEvt->m_trigTime
      << ", det=" << m_lastEvt->m_det
      << endl;
    m_timeGap += dt;
    double timeGap = event->m_trigTime - m_liveTimeSvc->startTime(m_timeBin);
    m_timeGapInCurBin += dt < timeGap ? dt : timeGap;
    m_liveTimeSvc->setFirstGapEventTrigtime(m_lastEvt->m_trigTime);
    m_liveTimeSvc->setLastGapEventTrigtime(event->m_trigTime);
    std::cout<<"dt > evtTimeGap "<<endl;
    std::cout<<"FirstGapEventTrigtime  : "<<m_liveTimeSvc->getFirstGapEventTrigtime()<<endl;
    std::cout<<"LastGapEventTrigtime  : "<<m_liveTimeSvc->getLastGapEventTrigtime()<<endl;
  }

  m_lastEvt->ReleaseInstance();
  m_lastEvt = event->GrabInstance();

  m_liveTimeSvc->setWallTime(m_endTime - m_beginTime);
  m_liveTimeSvc->setDaqTime((m_endTime - m_beginTime) - m_timeGap);
  //for(int det=1;det<=GlobalVar::NumADs;det++) {
  //  m_liveTimeSvc->setLiveTime(det, (m_endTime - m_beginTime) 
  //      - m_timeGap - m_muonVeto->m_deadTime[det-1]);
  //}

   // std::cout<<"done FirstGapEventTrigtime  : "<<setprecision(20)<<m_liveTimeSvc->getFirstGapEventTrigtime()<<endl;
   // std::cout<<"done LastGapEventTrigtime  : "<<m_liveTimeSvc->getLastGapEventTrigtime()<<endl;
  /*
    if( event->m_det<5 && !m_muonVeto->veto(event))
    {
        for( int i=1 ; i<3 ; i++ )
        {
      if( m_muonVeto->m_SingleLiveTime200[i-1]>(m_liveTimeSvc->daqTime()- m_muonVeto->m_deadTime[i-1]) || 1)
      {
          std::cout<<"det  : "<<i<<endl;
          std::cout<<"trigTime  : "<<event->m_trigTime<<endl;
          std::cout<<"fileNum  : "<<event->m_fileNum<<endl;
          std::cout<<"local entry  : "<<event->m_localEntry<<endl;
          std::cout<<"SingleLiveTime200>liveTime  : "<< m_muonVeto->m_SingleLiveTime200[i-1]-(m_liveTimeSvc->daqTime()- m_muonVeto->m_deadTime[i-1])<<endl;
      }
        }
        
        
    }
*/
  return true;
}
