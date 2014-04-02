#include "MuonVeto/MuonVeto.h"
#include "LafKernel/SvcMgr.h"
#include "LafKernel/LafLog.h"
#include "LafKernel/OptionParser.h"
#include "LafKernel/GlobalVar.h"

using namespace std;

MuonVeto* MuonVeto::m_instance = 0;

MuonVeto* MuonVeto::instance()
{
  if(!m_instance) m_instance = new MuonVeto();
  return m_instance;
}

MuonVeto::MuonVeto()
    : m_name("MuonVeto")
{
  m_buffer = SvcMgr::get<PhyEventBuf>("Cycler");
  m_liveTimeSvc =SvcMgr::get<LiveTimeSvc>("LiveTimeSvc");
  /*
  if(!m_liveTimeSvc) {
    LogError << "Can't get LiveTimeSvc." << endl;
    return false;
  }
  */
  for(int det=1; det<=GlobalVar::NumADs; det++) {
    m_deadTime[det-1] = 0.;
    m_deadTimeOld[det-1] = 0.;
    m_vetoWindowPost[det-1] = 0;
    m_vetoWindowPostOld[det-1] = 0;
    m_deadTime4IbdSel[det-1] = 0.;
    m_deadTime4IbdSelOld[det-1] = 0.;
    m_vetoWindowPost4IbdSel[det-1] = 0;
    m_vetoWindowPost4IbdSelOld[det-1] = 0;
    m_SingleLiveTime[det-1]=0.;
    m_SingleLiveTimeOld[det-1]=0.;
    m_SingleLiveTime200[det-1]=0.;
    m_SingleLiveTime200Old[det-1]=0.;
    //vetoWindowPost_temp[det-1]=0.;
    //m_deadTime_temp[det-1]=0.;
  }
  //for livetime
  OptionParser::setOption("MuonVeto","MuVetoWindow",m_muVetoWin=6.e-4);
  OptionParser::setOption("MuonVeto","AdMuVetoWindow",m_adMuVetoWin=1.e-3);
  OptionParser::setOption("MuonVeto","ShowerMuVetoWindow",m_showerMuVetoWin=1.);
  OptionParser::setOption("MuonVeto","PreVetoWindow",m_preVetoWin=2.e-6);
  //for veto events,,is different from upper in IBD seclection
  OptionParser::setOption("MuonVeto","MuVetoWindow4IbdSel",m_muVetoWin4IbdSel=2.e-4);
  OptionParser::setOption("MuonVeto","AdMuVetoWindow4IbdSel",m_adMuVetoWin4IbdSel=2.e-4);
  OptionParser::setOption("MuonVeto","ShowerMuVetoWindow4IbdSel",m_showerMuVetoWin4IbdSel=2.e-4);
  //OptionParser::setOption("MuonVeto","PreVetoWindow4IbdSel",m_preVetoWin4IbdSel=2.e-6);
  OptionParser::setOption("MuonVeto", "useRPCVeto", m_useRPCVeto=true);
}

void MuonVeto::updateVetoWindow(PhyEvent *event)
{
  //PhyEvent *event = m_buffer->curEvt();


  //for(int det=1;det<=GlobalVar::NumAdOfSite[event->m_site];det++) {
    //double newVetoWindowPre_temp=0.;
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    if(!event->isMuon()) continue;
    LogDebug << "det=" << det << "," << event->m_muonTag << std::endl;

    TTimeStamp newVetoWindowPost(event->m_trigTime);
    TTimeStamp newVetoWindowPostOld(event->m_trigTime);
    TTimeStamp newVetoWindowPost4IbdSel(event->m_trigTime);
    TTimeStamp newVetoWindowPost4IbdSelOld(event->m_trigTime);
    if(event->isWpMuon()) {
      newVetoWindowPost.Add(timeCnv(m_muVetoWin));
      newVetoWindowPostOld.Add(timeCnv(m_muVetoWin));
      newVetoWindowPost4IbdSel.Add(timeCnv(m_muVetoWin4IbdSel));
      newVetoWindowPost4IbdSelOld.Add(timeCnv(m_muVetoWin4IbdSel));
    } else if(event->isRpcMuon() && m_useRPCVeto) {
      newVetoWindowPost.Add(timeCnv(m_muVetoWin));
      newVetoWindowPost4IbdSel.Add(timeCnv(m_muVetoWin4IbdSel));
    } else if(event->isAdMuon() && event->m_det==det) { 
      newVetoWindowPost.Add(timeCnv(m_adMuVetoWin));
      newVetoWindowPostOld.Add(timeCnv(m_adMuVetoWin));
      newVetoWindowPost4IbdSel.Add(timeCnv(m_adMuVetoWin4IbdSel));
      newVetoWindowPost4IbdSelOld.Add(timeCnv(m_adMuVetoWin4IbdSel));
    } else if(event->isAdMuonNew() && event->m_det==det) { 
      newVetoWindowPost.Add(timeCnv(m_adMuVetoWin));
      newVetoWindowPost4IbdSel.Add(timeCnv(m_adMuVetoWin4IbdSel));
    } else if(event->isShowerMuon() && event->m_det==det) {
      newVetoWindowPost.Add(timeCnv(m_showerMuVetoWin));
      newVetoWindowPostOld.Add(timeCnv(m_showerMuVetoWin));
      newVetoWindowPost4IbdSel.Add(timeCnv(m_showerMuVetoWin4IbdSel));
      newVetoWindowPost4IbdSelOld.Add(timeCnv(m_showerMuVetoWin4IbdSel));
    } else if(event->isShowerMuonNew() && event->m_det==det) {
      newVetoWindowPost.Add(timeCnv(m_showerMuVetoWin));
      newVetoWindowPost4IbdSel.Add(timeCnv(m_showerMuVetoWin4IbdSel));
    } else {
      continue;
    }

    TTimeStamp newVetoWindowPre(event->m_trigTime);
    TTimeStamp newVetoWindowPreOld(event->m_trigTime);
    TTimeStamp newVetoWindowPre4IbdSel(event->m_trigTime);
    TTimeStamp newVetoWindowPre4IbdSelOld(event->m_trigTime);
    // Apply pre veto window for WP or RPC muon
    if(event->isWpMuon()) {
      newVetoWindowPre.Add(timeCnv(-m_preVetoWin));
      newVetoWindowPreOld.Add(timeCnv(-m_preVetoWin));
      newVetoWindowPre4IbdSel.Add(timeCnv(-m_preVetoWin));
      newVetoWindowPre4IbdSelOld.Add(timeCnv(-m_preVetoWin));
    }
    if(event->isRpcMuon()&& m_useRPCVeto) {
      newVetoWindowPre.Add(timeCnv(-m_preVetoWin));
      newVetoWindowPre4IbdSel.Add(timeCnv(-m_preVetoWin));
    }

    LogDebug << "Current time=" << event->m_trigTime
      << ", veto window pre=" << newVetoWindowPre
      << ", veto window post=" << newVetoWindowPost << endl;
    /*
    if( newVetoWindowPre> m_vetoWindowPost[det-1] )
    {
        newVetoWindowPre_temp=newVetoWindowPre.AsDouble();
     vetoWindowPost_temp[det-1]=m_vetoWindowPost[det-1].AsDouble();
     std::cout<<"m_vetoWindowPost["<<det<<"]  : "<<m_vetoWindowPost[det-1].AsDouble()<<endl;
     std::cout<<"m_deadTime["<<det<<"]  : "<<m_deadTime[det-1]<<endl;
     m_deadTime_temp[det-1]=m_deadTime[det-1];
    }
    */
    if(newVetoWindowPost > m_vetoWindowPost[det-1]) {
      LogDebug << "Update muon veto window to " << newVetoWindowPost
        << " by event " << event->m_entry
        << " in detector " << event->m_det << endl;
      if(newVetoWindowPre > m_vetoWindowPost[det-1]) {
        m_deadTime[det-1] += (newVetoWindowPost-newVetoWindowPre);
        m_deadTime4IbdSel[det-1] += (newVetoWindowPost4IbdSel-newVetoWindowPre4IbdSel);
        double livetimewindow=0.;
        if( m_liveTimeSvc->getLastGapEventTrigtime()>m_vetoWindowPost[det-1].AsDouble() )
        {
            livetimewindow=newVetoWindowPre.AsDouble()-m_liveTimeSvc->getLastGapEventTrigtime();
            if( livetimewindow>0. )
            {
                if( livetimewindow<2.e-4 )
                {
                    m_SingleLiveTime[det-1]+=livetimewindow;
                } else
                {
                    m_liveTimeSvc->setNumberOfSingleLiveTime200(det,m_liveTimeSvc->getNumberOfSingleLiveTime200(det)+1);
                    m_SingleLiveTime200[det-1]+=livetimewindow;
                }
                /*
                if( livetimewindow>0.1 )
                {
                    std::cout<<"1 "<<endl; 
                    std::cout<<"livetimewindow  : "<<livetimewindow<<endl;
                }
                */
            }
            if( m_liveTimeSvc->getFirstGapEventTrigtime()>m_vetoWindowPost[det-1].AsDouble() && m_vetoWindowPost[det-1].AsDouble()!=0)
            {
                livetimewindow=m_liveTimeSvc->getFirstGapEventTrigtime()-m_vetoWindowPost[det-1].AsDouble();
                if( livetimewindow>0. )
                {
                    if( livetimewindow<2.e-4 )
                    {
                        m_SingleLiveTime[det-1]+=livetimewindow;
                    } else
                    {
                        m_liveTimeSvc->setNumberOfSingleLiveTime200(det,m_liveTimeSvc->getNumberOfSingleLiveTime200(det)+1);
                        m_SingleLiveTime200[det-1]+=livetimewindow;
                    }
                    /*
                if( livetimewindow>0.1 )
                {
                    std::cout<<"2 "<<endl; 
                    std::cout<<"livetimewindow  : "<<livetimewindow<<endl;
                }
                */
                }
            }else
            {
                //std::cout<<"FirstGapEventTrigtime<m_vetoWindowPost[det-1],wrong stat,but no need to fix it."<<endl;
            }
        } else
        {
            livetimewindow=newVetoWindowPre-m_vetoWindowPost[det-1];
            if( livetimewindow>0. )
            {
                if( livetimewindow<2.e-4 )
                {
                    m_SingleLiveTime[det-1]+=livetimewindow;
                } else
                {
                    m_liveTimeSvc->setNumberOfSingleLiveTime200(det,m_liveTimeSvc->getNumberOfSingleLiveTime200(det)+1);
                    m_SingleLiveTime200[det-1]+=livetimewindow;
                }
                /*
                if( livetimewindow>0.1 )
                {
                    std::cout<<"3 "<<endl; 
                    std::cout<<"livetimewindow  : "<<livetimewindow<<endl;
                }
                */
            }
        }

        LogDebug << (newVetoWindowPost-event->m_trigTime) << std::endl;
      } else {
        m_deadTime[det-1] += (newVetoWindowPost-m_vetoWindowPost[det-1]);
        m_deadTime4IbdSel[det-1] += (newVetoWindowPost4IbdSel-m_vetoWindowPost4IbdSel[det-1]);
      }
      m_vetoWindowPost[det-1] = newVetoWindowPost;
    }
    if(newVetoWindowPost4IbdSel > m_vetoWindowPost4IbdSel[det-1]) {
      m_vetoWindowPost4IbdSel[det-1] = newVetoWindowPost4IbdSel;
    }

    if(newVetoWindowPostOld > m_vetoWindowPostOld[det-1]) {
      LogDebug << "Update muon veto window to " << newVetoWindowPostOld
        << " by event " << event->m_entry
        << " in detector " << event->m_det << endl;
      if(newVetoWindowPreOld > m_vetoWindowPostOld[det-1]) {
        //m_liveTimeOld[det-1] += (newVetoWindowPreOld-m_vetoWindowPostOld[det-1]);
        m_deadTimeOld[det-1] += (newVetoWindowPostOld-newVetoWindowPreOld);
        m_deadTime4IbdSelOld[det-1] += (newVetoWindowPost4IbdSelOld-newVetoWindowPre4IbdSelOld);
        double livetimewindowOld=0.;
        if( m_liveTimeSvc->getLastGapEventTrigtime()>m_vetoWindowPostOld[det-1].AsDouble() )
        {
            livetimewindowOld=newVetoWindowPreOld.AsDouble()-m_liveTimeSvc->getLastGapEventTrigtime();
            if( livetimewindowOld>0. )
            {
                if( livetimewindowOld<2.e-4 )
                {
                    m_SingleLiveTimeOld[det-1]+=livetimewindowOld;
                } else
                {
                    m_liveTimeSvc->setNumberOfSingleLiveTime200Old(det,m_liveTimeSvc->getNumberOfSingleLiveTime200Old(det)+1);
                    m_SingleLiveTime200Old[det-1]+=livetimewindowOld;
                }
            }
            if(m_liveTimeSvc->getFirstGapEventTrigtime()>m_vetoWindowPostOld[det-1].AsDouble() && m_vetoWindowPostOld[det-1].AsDouble()!=0. )
            {
                livetimewindowOld=m_liveTimeSvc->getFirstGapEventTrigtime()-m_vetoWindowPostOld[det-1].AsDouble();

                if( livetimewindowOld>0. )
                {
                    if( livetimewindowOld<2.e-4 )
                    {
                        m_SingleLiveTimeOld[det-1]+=livetimewindowOld;
                    } else
                    {
                        m_liveTimeSvc->setNumberOfSingleLiveTime200Old(det,m_liveTimeSvc->getNumberOfSingleLiveTime200Old(det)+1);
                        m_SingleLiveTime200Old[det-1]+=livetimewindowOld;
                    }
                }
            }else
            {
                //std::cout<<"FirstGapEventTrigtime<m_vetoWindowPostOld[det-1],wrong stat,but no need to fix it."<<endl;
            }
        } else
        {
            livetimewindowOld=newVetoWindowPreOld-m_vetoWindowPostOld[det-1];
            if( livetimewindowOld>0. )
            {
                if( livetimewindowOld<2.e-4 )
                {
                    m_SingleLiveTimeOld[det-1]+=livetimewindowOld;
                } else
                {
                    m_liveTimeSvc->setNumberOfSingleLiveTime200Old(det,m_liveTimeSvc->getNumberOfSingleLiveTime200Old(det)+1);
                    m_SingleLiveTime200Old[det-1]+=livetimewindowOld;
                }
            }
        }
        LogDebug << (newVetoWindowPostOld-event->m_trigTime) << std::endl;
      } else {
        m_deadTimeOld[det-1] += (newVetoWindowPostOld-m_vetoWindowPostOld[det-1]);
        m_deadTime4IbdSelOld[det-1] += (newVetoWindowPost4IbdSelOld-m_vetoWindowPost4IbdSelOld[det-1]);
      }
      m_vetoWindowPostOld[det-1] = newVetoWindowPostOld;
    }
    if(newVetoWindowPost4IbdSelOld > m_vetoWindowPost4IbdSelOld[det-1]) {
      m_vetoWindowPost4IbdSelOld[det-1] = newVetoWindowPost4IbdSelOld;
    }
  }
  /*
  for( int i=1 ; i<3 ; i++ )
  {
    if( newVetoWindowPre_temp > vetoWindowPost_temp[i-1])
    {
        std::cout<<"first muon "<<i<<endl;
        std::cout<<"SingleLiveTime200  : "<<m_SingleLiveTime200[i-1]<<endl;
        std::cout<<"daqTime  : "<<m_liveTimeSvc->daqTime()<<endl;
        std::cout<<"deadTime  : "<<m_deadTime_temp[i-1]<<endl;
        std::cout<<"liveTime  : "<<m_liveTimeSvc->daqTime()-m_deadTime_temp[i-1]<<endl;
      if( m_SingleLiveTime200[i-1]>(m_liveTimeSvc->daqTime()-m_deadTime_temp[i-1]))
      {
          std::cout<<"det  : "<<i<<endl;
          std::cout<<"trigTime  : "<<event->m_trigTime<<endl;
          std::cout<<"fileNum  : "<<event->m_fileNum<<endl;
          std::cout<<"local entry  : "<<event->m_localEntry<<endl;
          std::cout<<"SingleLiveTime200>liveTime  : "<< m_SingleLiveTime200[i-1]-(m_liveTimeSvc->daqTime()-m_deadTime_temp[i-1])<<endl;
      }
    }
        
 }
*/
  
}

int MuonVeto::veto(PhyEvent *event)
{
  //PhyEvent *event = m_buffer->curEvt();
  if(event->m_trigTime < m_vetoWindowPost4IbdSelOld[event->m_det-1]) {
    LogDebug << "Current time=" << event->m_trigTime
      << ", veto window=" << m_vetoWindowPost4IbdSelOld[event->m_det-1]
      <<endl;
    return 1;
  } 
  
	// Make sure there is no WP  muon in following small time window
	PhyEvent *nextMuon=0;
	//DataBuffer<PhyEvent>::Iterator it=m_buffer->current();
	DataBuffer<PhyEvent>::Iterator it=m_buffer->find(event);
	it++;
	for(;it!=m_buffer->end();it++) {
		if((*it)->isWpMuon() && (*it)->m_good) { // find a muon
			//if((*it)->isWP() ) { // WP  muon
			nextMuon = (*it);
			break;
			//}
		}
	}
	if(nextMuon) {
	LogDebug << "Current entry: " << event->m_entry
       << ", next muon entry and trigger type: " << nextMuon->m_entry 
       << "," << hex << nextMuon->m_trigType << dec << endl;
	//float t2nextMu = nextMuon->m_trigTime - event->m_trigTime;
	double t2nextMu = nextMuon->m_trigTime - event->m_trigTime;
	LogDebug << "Time to next muon: " << t2nextMu << endl;
		if(t2nextMu <= m_preVetoWin) {
			LogDebug << "Veto this event in front of muon." << endl;
			return 1;
		}
	}
  
	if( event->m_trigTime < m_vetoWindowPost4IbdSel[event->m_det-1] )
	{
		return 2;	
	} 
	
	// Make sure there is no RPC muon in following small time window
	PhyEvent *nextRpcMuon=0;
	DataBuffer<PhyEvent>::Iterator itRpc=m_buffer->find(event);
	itRpc++;
	for(;itRpc!=m_buffer->end();itRpc++) {
		if((*itRpc)->isRpcMuon() && (*itRpc)->m_good && m_useRPCVeto) { // find a muon
			//if(((*itRpc)->isRPC() && m_useRPCVeto)) { //  RPC muon
			nextRpcMuon = (*itRpc);
			break;
			//}
		}
	}

	if(nextRpcMuon) {
	LogDebug << "Current entry: " << event->m_entry
        << ", next muon entry and trigger type: " << nextRpcMuon->m_entry 
        << "," << hex << nextRpcMuon->m_trigType << dec << endl;
	double t2nextRpcMu = nextRpcMuon->m_trigTime - event->m_trigTime;
	LogDebug << "Time to next muon: " << t2nextRpcMu << endl;
		if(t2nextRpcMu <= m_preVetoWin) {
			LogDebug << "Veto this event in front of muon." << endl;
			return 2;
		}
	}

	return 0;
}

PhyEvent* MuonVeto::preMuon(PhyEvent *event, const int det)
{
  //PhyEventBuf::Iterator it = m_buffer->current();
  DataBuffer<PhyEvent>::Iterator it=m_buffer->find(event);
  LogDebug << "Current entry: " << (*it)->m_entry << endl;
  it--;
  while ( it >= m_buffer->begin() ) {
    PhyEvent* event = (*it);
    if(event->isMuon()) {
      if(event->m_det==det || det==0) {
        LogDebug << "Last muon entry: " << event->m_entry << endl;
        return event;
      }
    }
    it--;
  }
  return 0;
}

PhyEvent* MuonVeto::nextMuon(PhyEvent *event, const int det)
{
  //PhyEventBuf::Iterator it = m_buffer->current();
  DataBuffer<PhyEvent>::Iterator it=m_buffer->find(event);
  LogDebug << "Current entry: " << (*it)->m_entry << endl;
  it++;
  while ( it < m_buffer->end() ) {
    PhyEvent* event = (*it);
    if(event->isMuon()) {
      if(event->m_det==det || det==0) {
        return event;
      }
    }
    it++;
  }
  return 0;
}

PhyEvent* MuonVeto::preAdMuon(PhyEvent *event, double eLow, double eHigh)
{
  DataBuffer<PhyEvent>::Iterator it=m_buffer->find(event);
  LogDebug << "Current entry: " << (*it)->m_entry << endl;
  int det = event->m_det;
  --it;
  while ( it >= m_buffer->begin() ) {
      PhyEvent* event = (*it);
      if ( event->isAdMuon() && event->m_det == det ) {
	  if ( event->energy() < eHigh && event->energy() > eLow ) {
	      return event;
	  }
      }
      --it;
  }
  return 0;
}

PhyEvent* MuonVeto::preAdMuonNew(PhyEvent *event, double eLow, double eHigh)
{
  DataBuffer<PhyEvent>::Iterator it=m_buffer->find(event);
  LogDebug << "Current entry: " << (*it)->m_entry << endl;
  int det = event->m_det;
  --it;
  while ( it >= m_buffer->begin() ) {
      PhyEvent* event = (*it);
      if ( event->isAdMuonNew() && event->m_det == det ) {
	  if ( event->energy() < eHigh && event->energy() > eLow ) {
	      return event;
	  }
      }
      --it;
  }
  return 0;
}

PhyEvent* MuonVeto::preShowerMuon(PhyEvent *event)
{
  DataBuffer<PhyEvent>::Iterator it=m_buffer->find(event);
  LogDebug << "Current entry: " << (*it)->m_entry << endl;
  int det = event->m_det;
  --it;
  while ( it >= m_buffer->begin() ) {
      PhyEvent* event = (*it);
      if ( event->isShowerMuon() && event->m_det == det ) {
	  return event;
      }
      --it;
  }
  return 0;
}

PhyEvent* MuonVeto::preShowerMuonNew(PhyEvent *event)
{
  DataBuffer<PhyEvent>::Iterator it=m_buffer->find(event);
  LogDebug << "Current entry: " << (*it)->m_entry << endl;
  int det = event->m_det;
  --it;
  while ( it >= m_buffer->begin() ) {
      PhyEvent* event = (*it);
      if ( event->isShowerMuonNew() && event->m_det == det ) {
	  return event;
      }
      --it;
  }
  return 0;
}

const TTimeStamp MuonVeto::timeCnv(double time)
{
  int sec = (int)time;
  int nanosec = (int)((time-sec)*1.e9);
  return TTimeStamp(sec,nanosec);
}

//DataBuffer<PhyEvent>::Iterator MuonVeto::iter(PhyEvent *event)
//{
//  int dentry = event->m_entry - m_buffer->curEvt()->m_entry;
//  return m_buffer->current() + dentry;
//}
