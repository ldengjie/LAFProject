#include "Algorithms/MuonTagAlg.h"
#include "MuonVeto/MuonVeto.h"
#include "LafKernel/GlobalVar.h"
#include "LiveTimeSvc/LiveTimeSvc.h"
#include <iostream>
#include <sstream>
#include <TH1F.h>
#include <TH2F.h>

using namespace std;

MuonTagAlg::MuonTagAlg(const std::string& name)
: AlgBase(name),m_timeBin(-1)
{
    m_hIwsMuonNumber=0;
    m_hOwsMuonNumber=0;
    m_hRpcMuonNumber=0;
    m_hIsoRpcMuonNumber=0;
  for(int det=1;det<=GlobalVar::NumADs;det++) {
      m_hAdMuonNumber[det-1] = 0;
      m_hShowerMuonNumber[det-1] = 0;
      m_hAdMuonNewNumber[det-1] = 0;
      m_hShowerMuonNewNumber[det-1] = 0;
    m_vetoTimeInCurBin[det-1] = 0.;
    m_vetoTimeInCurBinOld[det-1] = 0.;
    m_vetoTimeAccum[det-1] = 0.;
    m_vetoTimeAccumOld[det-1] = 0.;
    m_vetoTimeInCurBin4IbdSel[det-1] = 0.;
    m_vetoTimeInCurBin4IbdSelOld[det-1] = 0.;
    m_vetoTimeAccum4IbdSel[det-1] = 0.;
    m_vetoTimeAccum4IbdSelOld[det-1] = 0.;
     m_SingleLiveTimeAccum[det-1] = 0.;
     m_SingleLiveTimeAccumOld[det-1] = 0.;
     m_SingleLiveTimeAccum200[det-1] = 0.;
     m_SingleLiveTimeAccum200Old[det-1] = 0.;
  }
}

bool MuonTagAlg::initialize()
{
  m_buffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
  m_muonVeto = MuonVeto::instance();
  m_liveTimeSvc = dynamic_cast<LiveTimeSvc*>(service("LiveTimeSvc"));
  if(!m_liveTimeSvc) {
    LogError << "Can't get LiveTimeSvc." << endl;
    return false;
  }

  m_hIwsMuonNum = new TH1F("IwsMuonNum","IWS muon number",
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  m_hOwsMuonNum = new TH1F("OwsMuonNum","OWS muon number",
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  m_hRpcMuonNum = new TH1F("RpcMuonNum","RPC muon number",
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  m_hIsoRpcMuonNum = new TH1F("IosRpcMuonNum","isolated RPC muon number",
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    stringstream ss;
    ss << "AD" << det << "MuonNum";
    m_hAdMuonNum[det-1] = new TH1F(ss.str().c_str(),ss.str().c_str(),
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  }
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    stringstream ss;
    ss << "AD" << det << "ShowerMuonNum";
    m_hShowerMuonNum[det-1] = new TH1F(ss.str().c_str(),ss.str().c_str(),
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  }
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    stringstream ss;
    ss << "AD" << det << "MuonNewNum";
    m_hAdMuonNewNum[det-1] = new TH1F(ss.str().c_str(),ss.str().c_str(),
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  }
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    stringstream ss;
    ss << "AD" << det << "ShowerMuonNewNum";
    m_hShowerMuonNewNum[det-1] = new TH1F(ss.str().c_str(),ss.str().c_str(),
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  }
  ntupleSvc()->attach("FILE1/Muon/",m_hIwsMuonNum);
  ntupleSvc()->attach("FILE1/Muon/",m_hOwsMuonNum);
  ntupleSvc()->attach("FILE1/Muon/",m_hRpcMuonNum);
  ntupleSvc()->attach("FILE1/Muon/",m_hIsoRpcMuonNum);
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    ntupleSvc()->attach("FILE1/Muon/",m_hAdMuonNum[det-1]);
    ntupleSvc()->attach("FILE1/Muon/",m_hShowerMuonNum[det-1]);
    ntupleSvc()->attach("FILE1/Muon/",m_hAdMuonNewNum[det-1]);
    ntupleSvc()->attach("FILE1/Muon/",m_hShowerMuonNewNum[det-1]);
  }

  m_hIwsMuon = new TH1F("IwsMuonRate","IWS muon rate",
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  m_hOwsMuon = new TH1F("OwsMuonRate","OWS muon rate",
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  m_hRpcMuon = new TH1F("RpcMuonRate","RPC muon rate",
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    stringstream ss;
    ss << "AD" << det << "MuonRate";
    m_hAdMuon[det-1] = new TH1F(ss.str().c_str(),ss.str().c_str(),
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  }
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    stringstream ss;
    ss << "AD" << det << "ShowerMuonRate";
    m_hShowerMuon[det-1] = new TH1F(ss.str().c_str(),ss.str().c_str(),
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  }
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    stringstream ss;
    ss << "AD" << det << "MuonNewRate";
    m_hAdMuonNew[det-1] = new TH1F(ss.str().c_str(),ss.str().c_str(),
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  }
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    stringstream ss;
    ss << "AD" << det << "ShowerMuonNewRate";
    m_hShowerMuonNew[det-1] = new TH1F(ss.str().c_str(),ss.str().c_str(),
      m_liveTimeSvc->nBins(),
      m_liveTimeSvc->startTime().AsDouble(),
      m_liveTimeSvc->endTime().AsDouble()
      );
  }
  ntupleSvc()->attach("FILE1/Muon/",m_hIwsMuon);
  ntupleSvc()->attach("FILE1/Muon/",m_hOwsMuon);
  ntupleSvc()->attach("FILE1/Muon/",m_hRpcMuon);
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    ntupleSvc()->attach("FILE1/Muon/",m_hAdMuon[det-1]);
    ntupleSvc()->attach("FILE1/Muon/",m_hShowerMuon[det-1]);
    ntupleSvc()->attach("FILE1/Muon/",m_hAdMuonNew[det-1]);
    ntupleSvc()->attach("FILE1/Muon/",m_hShowerMuonNew[det-1]);
  }

  return true;
}

bool MuonTagAlg::finalize()
{
  // set live time for the last time bin
    doNewBin(m_timeBin);
  cout << "Wall time :" << m_liveTimeSvc->wallTime() << "s, "
    << "DAQ data taking time: " << m_liveTimeSvc->daqTime()
    << "s, Live time and ratio: ";
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    cout << "AD" << det << ":(" << m_liveTimeSvc->liveTime(det) << "s,"
      << m_liveTimeSvc->liveTime(det)/m_liveTimeSvc->daqTime() << "),";
  }
  cout<<"Not use Rpc Veto : "<<endl;
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    cout << "AD" << det << ":(" << m_liveTimeSvc->liveTimeOld(det) << "s,"
      << m_liveTimeSvc->liveTimeOld(det)/m_liveTimeSvc->daqTime() << "),";
  }
  cout << endl;

  cout << "DAQ time, live time and ratio for each time bin:" << endl;
  for(int i=0;i<m_liveTimeSvc->nBins();i++) {
    if(m_liveTimeSvc->daqTimePerBin(i) > 0.) {
      cout << "Bin " << i+1 << "(" << m_liveTimeSvc->startTime(i) << " to "
        << m_liveTimeSvc->startTime(i+1)
        << "):\n DAQ time=" << m_liveTimeSvc->daqTimePerBin(i) << "s, live time=";
      for(int det=1;det<=GlobalVar::NumADs;det++) {
        cout << "AD" << det << ":(" << m_liveTimeSvc->liveTimePerBin(det,i) << "s,"
          << m_liveTimeSvc->liveTimePerBin(det,i)/m_liveTimeSvc->daqTimePerBin(i) << "),";
		cout<<"Not use Rpc Veto"<<endl;
        cout << "AD" << det << ":(" << m_liveTimeSvc->liveTimePerBinOld(det,i) << "s,"
          << m_liveTimeSvc->liveTimePerBinOld(det,i)/m_liveTimeSvc->daqTimePerBin(i) << "),";
      }
      cout << endl;
    }
  }

  for(int i=0;i<m_liveTimeSvc->nBins();i++) {
    if(m_liveTimeSvc->daqTimePerBin(i) > 0.) {
        /*
      m_hIwsMuon->Fill(m_liveTimeSvc->startTime(i) + m_liveTimeSvc->binWidth()/2,
          m_hIwsMuonNum->GetBinContent(i+1)/m_liveTimeSvc->daqTimePerBin(i));
      m_hOwsMuon->Fill(m_liveTimeSvc->startTime(i) + m_liveTimeSvc->binWidth()/2,
          m_hOwsMuonNum->GetBinContent(i+1)/m_liveTimeSvc->daqTimePerBin(i));
      m_hRpcMuon->Fill(m_liveTimeSvc->startTime(i) + m_liveTimeSvc->binWidth()/2,
          m_hRpcMuonNum->GetBinContent(i+1)/m_liveTimeSvc->daqTimePerBin(i));
      for(int det=1;det<=GlobalVar::NumADs;det++) {
        m_hAdMuon[det-1]->Fill(m_liveTimeSvc->startTime(i) + m_liveTimeSvc->binWidth()/2,
            m_hAdMuonNum[det-1]->GetBinContent(i+1)/m_liveTimeSvc->daqTimePerBin(i));
        m_hShowerMuon[det-1]->Fill(m_liveTimeSvc->startTime(i) + m_liveTimeSvc->binWidth()/2,
            m_hShowerMuonNum[det-1]->GetBinContent(i+1)/m_liveTimeSvc->daqTimePerBin(i));
        m_hAdMuonNew[det-1]->Fill(m_liveTimeSvc->startTime(i) + m_liveTimeSvc->binWidth()/2,
            m_hAdMuonNewNum[det-1]->GetBinContent(i+1)/m_liveTimeSvc->daqTimePerBin(i));
        m_hShowerMuonNew[det-1]->Fill(m_liveTimeSvc->startTime(i) + m_liveTimeSvc->binWidth()/2,
            m_hShowerMuonNewNum[det-1]->GetBinContent(i+1)/m_liveTimeSvc->daqTimePerBin(i));
      }
      */
        //std::cout<<"i  : "<<i<<endl;
        //std::cout<<"m_hIwsMuonNum->GetBinContent(i)  : "<<m_hIwsMuonNum->GetBinContent(i)<<endl;
        //std::cout<<"m_liveTimeSvc->daqTimePerBin(i)  : "<<m_liveTimeSvc->daqTimePerBin(i)<<endl;
        m_hIwsMuon->SetBinContent(i,m_hIwsMuonNum->GetBinContent(i)/m_liveTimeSvc->daqTimePerBin(i));
        m_hOwsMuon->SetBinContent(i,m_hOwsMuonNum->GetBinContent(i)/m_liveTimeSvc->daqTimePerBin(i));
        m_hRpcMuon->SetBinContent(i,m_hRpcMuonNum->GetBinContent(i)/m_liveTimeSvc->daqTimePerBin(i));
        for(int det=1;det<=GlobalVar::NumADs;det++) 
        {
            m_hAdMuon[det-1]->SetBinContent(i,m_hAdMuonNum[det-1]->GetBinContent(i)/m_liveTimeSvc->daqTimePerBin(i));
            m_hShowerMuon[det-1]->SetBinContent(i,m_hShowerMuonNum[det-1]->GetBinContent(i)/m_liveTimeSvc->daqTimePerBin(i));
            m_hAdMuonNew[det-1]->SetBinContent(i,m_hAdMuonNewNum[det-1]->GetBinContent(i)/m_liveTimeSvc->daqTimePerBin(i));
            m_hShowerMuonNew[det-1]->SetBinContent(i,m_hShowerMuonNewNum[det-1]->GetBinContent(i)/m_liveTimeSvc->daqTimePerBin(i));
        }
        
    }
  }

  return true;
}

bool MuonTagAlg::execute()
{
  PhyEvent *event = m_buffer->curEvt();

  int bin = m_liveTimeSvc->bin(event->m_trigTime);
  if(m_timeBin == -1) { // the first event
    m_timeBin = bin;
  } else {
    if(bin > m_timeBin) { // a new time bin

        doNewBin(m_timeBin);
      m_timeBin = bin;
    }
  }

  if(event->isMuon()) {
    LogDebug << "This is a muon: " << event->m_muonTag << endl;
    m_muonVeto->updateVetoWindow(event);
  }

  for(int det=1;det<=GlobalVar::NumADs;det++) {
    m_liveTimeSvc->setLiveTime(det, m_liveTimeSvc->daqTime() - m_muonVeto->m_deadTime[det-1]);
    m_liveTimeSvc->setLiveTimeOld(det, m_liveTimeSvc->daqTime() - m_muonVeto->m_deadTimeOld[det-1]);
    m_liveTimeSvc->setLiveTime4IbdSel(det, m_liveTimeSvc->daqTime() - m_muonVeto->m_deadTime4IbdSel[det-1]);
    m_liveTimeSvc->setLiveTime4IbdSelOld(det, m_liveTimeSvc->daqTime() - m_muonVeto->m_deadTime4IbdSelOld[det-1]);
  }

  if(event->isMuon() && event->m_det==5) {
    //m_hIwsMuonNum->Fill(event->m_trigTime.AsDouble());
    m_hIwsMuonNumber++;
  } else if(event->isMuon() && event->m_det==6) {
    //m_hOwsMuonNum->Fill(event->m_trigTime.AsDouble());
      m_hOwsMuonNumber++;
  } else if(event->isRpcMuon()) {
    //m_hRpcMuonNum->Fill(event->m_trigTime.AsDouble());
      m_hRpcMuonNumber++;
    bool isIso=true;
	PhyEventBuf::Iterator it = m_buffer->current();
		  while ( --it >= m_buffer->begin() ) {
		    PhyEvent* _evt = *it;
		    if ( (_evt->m_trigTime - event->m_trigTime) > -2.e-6 )
	        {
			    if (  _evt->isMuon() ) {
                    isIso=false;
                    break;
			    }
	        }
		  }
          if( isIso )
          {
			  it = m_buffer->current();
              it++;
			  while ( it < m_buffer->end() ) {
			    PhyEvent* _evt = *it;
			    if ( (_evt->m_trigTime - event->m_trigTime) < 2.e-6 )
		        {
	                if (  _evt->isMuon() ) {
	                    isIso=false;
                        break;
	                }
		        }
                it++;
			  }
          }
          if( isIso )
          {
              //m_hIsoRpcMuonNum->Fill(event->m_trigTime.AsDouble());
              m_hIsoRpcMuonNumber++;
          }
  } else if(event->isAdMuon()) {
    //m_hAdMuonNum[event->m_det-1]->Fill(event->m_trigTime.AsDouble());
      m_hAdMuonNumber[event->m_det-1]++;
    //m_hAdMuonNewNum[event->m_det-1]->Fill(event->m_trigTime.AsDouble());
      m_hAdMuonNewNumber[event->m_det-1]++;
  } else if(event->isShowerMuon()) {
    //m_hShowerMuonNum[event->m_det-1]->Fill(event->m_trigTime.AsDouble());
      m_hShowerMuonNumber[event->m_det-1]++;
    //m_hShowerMuonNewNum[event->m_det-1]->Fill(event->m_trigTime.AsDouble());
      m_hShowerMuonNewNumber[event->m_det-1]++;
  } else if(event->isAdMuonNew()) {
    //m_hAdMuonNewNum[event->m_det-1]->Fill(event->m_trigTime.AsDouble());
      m_hAdMuonNewNumber[event->m_det-1]++;
  } else if(event->isShowerMuonNew()) {
    //m_hShowerMuonNewNum[event->m_det-1]->Fill(event->m_trigTime.AsDouble());
      m_hShowerMuonNewNumber[event->m_det-1]++;
  }

  return true;
}

void MuonTagAlg::doNewBin(int bin)
{
    m_hIwsMuonNum->SetBinContent(bin,m_hIwsMuonNumber);
    m_hOwsMuonNum->SetBinContent(bin,m_hOwsMuonNumber);
    m_hRpcMuonNum->SetBinContent(bin,m_hRpcMuonNumber);
    m_hIsoRpcMuonNum->SetBinContent(bin,m_hIsoRpcMuonNumber);
    m_hIwsMuonNumber=0;
    m_hOwsMuonNumber=0;
    m_hRpcMuonNumber=0;
    m_hIsoRpcMuonNumber=0;
    for(int det=1;det<=GlobalVar::NumADs;det++) 
    {
        m_hAdMuonNum[det-1]->SetBinContent(bin,m_hAdMuonNumber[det-1]);
        m_hAdMuonNewNum[det-1]->SetBinContent(bin,m_hAdMuonNewNumber[det-1]);
        m_hShowerMuonNum[det-1]->SetBinContent(bin,m_hShowerMuonNumber[det-1]);
        m_hShowerMuonNewNum[det-1]->SetBinContent(bin,m_hShowerMuonNewNumber[det-1]);
        
        m_hAdMuonNumber[det-1]=0;
        m_hAdMuonNewNumber[det-1]=0;
        m_hShowerMuonNumber[det-1]=0;
        m_hShowerMuonNewNumber[det-1]=0;
    }
    

      for(int det=1;det<=GlobalVar::NumADs;det++) {
          //std::cout<<" "<<endl;
          //std::cout<<"daqTime  : "<<m_liveTimeSvc->daqTimePerBin(m_timeBin)<<endl;
          //std::cout<<"deadTime  : "<<m_muonVeto->m_deadTime[det-1]<<endl;
          //std::cout<<"vetoTimeAccum  : "<<m_vetoTimeAccum[det-1]<<endl;
        double vetoTimeInCurBin = m_muonVeto->m_deadTime[det-1] - m_vetoTimeAccum[det-1];
        double liveTimeInCurBin = m_liveTimeSvc->daqTimePerBin(m_timeBin) - vetoTimeInCurBin;
        //cout << "AD" << det << ",bin" << m_timeBin << ":" 
        //  << m_vetoTimeAccum[det-1] << ","
        //  << vetoTimeInCurBin << endl;
        m_liveTimeSvc->setLiveTimePerBin(det,m_timeBin,liveTimeInCurBin);
        m_vetoTimeAccum[det-1] = m_muonVeto->m_deadTime[det-1];
		
		double vetoTimeInCurBinOld = m_muonVeto->m_deadTimeOld[det-1] - m_vetoTimeAccumOld[det-1];
		double liveTimeInCurBinOld = m_liveTimeSvc->daqTimePerBin(m_timeBin) - vetoTimeInCurBinOld;
		m_liveTimeSvc->setLiveTimePerBinOld(det,m_timeBin,liveTimeInCurBinOld);
		m_vetoTimeAccumOld[det-1] = m_muonVeto->m_deadTimeOld[det-1];
        
        double vetoTimeInCurBin4IbdSel = m_muonVeto->m_deadTime4IbdSel[det-1] - m_vetoTimeAccum4IbdSel[det-1];
        double liveTimeInCurBin4IbdSel = m_liveTimeSvc->daqTimePerBin(m_timeBin) - vetoTimeInCurBin4IbdSel;
        m_liveTimeSvc->setLiveTimePerBin4IbdSel(det,m_timeBin,liveTimeInCurBin4IbdSel);
        m_vetoTimeAccum4IbdSel[det-1] = m_muonVeto->m_deadTime4IbdSel[det-1];
    
	    double vetoTimeInCurBin4IbdSelOld = m_muonVeto->m_deadTime4IbdSelOld[det-1] - m_vetoTimeAccum4IbdSelOld[det-1];
        double liveTimeInCurBin4IbdSelOld = m_liveTimeSvc->daqTimePerBin(m_timeBin) - vetoTimeInCurBin4IbdSelOld;
        m_liveTimeSvc->setLiveTimePerBin4IbdSelOld(det,m_timeBin,liveTimeInCurBin4IbdSelOld);
        m_vetoTimeAccum4IbdSelOld[det-1] = m_muonVeto->m_deadTime4IbdSelOld[det-1];

        double singleLiveTimeInCurBin=m_muonVeto->m_SingleLiveTime[det-1]-m_SingleLiveTimeAccum[det-1];
        double singleLiveTimeInCurBin200=m_muonVeto->m_SingleLiveTime200[det-1]-m_SingleLiveTimeAccum200[det-1];
        double singleLiveTimeInCurBinOld=m_muonVeto->m_SingleLiveTimeOld[det-1]-m_SingleLiveTimeAccumOld[det-1];
        double singleLiveTimeInCurBin200Old=m_muonVeto->m_SingleLiveTime200Old[det-1]-m_SingleLiveTimeAccum200Old[det-1];
        //std::cout<<"SingleLiveTime200  : "<<m_muonVeto->m_SingleLiveTime200[det-1]<<endl;
        //std::cout<<"SingleLiveTimeAccum200  : "<<m_SingleLiveTimeAccum200[det-1]<<endl;
        m_liveTimeSvc->setSingleLiveTimePerBin(det,m_timeBin,singleLiveTimeInCurBin);
        m_liveTimeSvc->setSingleLiveTimePerBinOld(det,m_timeBin,singleLiveTimeInCurBinOld);
        m_liveTimeSvc->setSingleLiveTimePerBin200(det,m_timeBin,singleLiveTimeInCurBin200);
        m_liveTimeSvc->setSingleLiveTimePerBin200Old(det,m_timeBin,singleLiveTimeInCurBin200Old);
        m_SingleLiveTimeAccum[det-1]=m_muonVeto->m_SingleLiveTime[det-1];
        m_SingleLiveTimeAccumOld[det-1]=m_muonVeto->m_SingleLiveTimeOld[det-1];
        m_SingleLiveTimeAccum200[det-1]=m_muonVeto->m_SingleLiveTime200[det-1];
        m_SingleLiveTimeAccum200Old[det-1]=m_muonVeto->m_SingleLiveTime200Old[det-1];
        //std::cout<<"det  : "<<det<<endl;
        //std::cout<<"singleLiveTimeInCurBin200  : "<<singleLiveTimeInCurBin200<<endl;
        //std::cout<<"singleLiveTimeInCurBin200Old  : "<<singleLiveTimeInCurBin200Old<<endl;
        //std::cout<<"liveTimeInCurBin  : "<<liveTimeInCurBin<<endl;
        //std::cout<<"liveTimeInCurBinOld  : "<<liveTimeInCurBinOld<<endl;
      }
}
