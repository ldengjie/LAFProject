#include "IBDSelSimpAlg.h"
#include "LafKernel/AlgFactory.h"
#include "LafKernel/GlobalVar.h"
#include "LafKernel/PhyEvent/RecReadout.h"
#include <iostream>
#include "TMath.h"
#include "TNtuple.h"

using namespace std;

DECLARE_ALGORITHM(IBDSelSimpAlg);

IBDSelSimpAlg::IBDSelSimpAlg(const std::string& name)
: AlgBase(name)
{
  setOption("PromptEngLow",m_eplow);  
  setOption("PromptEngHigh",m_ephigh);  
  setOption("DelayedEngLow",m_edlow);  
  setOption("DelayedEngHigh",m_edhigh);  
  setOption("DtLow",m_dtlow);  
  setOption("DtHigh",m_dthigh);  
  OptionParser::setOption(name, "MuVetoWindow",m_muVetoWin=2.e-4);
  OptionParser::setOption(name, "AdMuVetoWindow",m_adMuVetoWin=1.e-3);
  OptionParser::setOption(name, "ShowerMuVetoWindow",m_showerMuVetoWin=1.0);
  OptionParser::setOption(name, "PreVetoWindow",m_preVetoWin=2.e-6);
  OptionParser::setOption(name, "drawHists", m_drawHists=true);
  OptionParser::setOption(name, "savePreAdMus", m_savePreAdMus=true);
  OptionParser::setOption(name, "AdMuonThreshold",m_adMuonThr=20.);
  OptionParser::setOption(name, "ShowerMuonThreshold",m_showerMuonThr=2500.);
  OptionParser::setOption(name, "useRPCVeto", m_useRPCVeto=true);
  OptionParser::setOption(name, "adMuonBufTh", m_adMuonBufThr=500.);
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    m_adBuf[det-1].clear();    
  }
  m_muonVeto = MuonVeto::instance();
    /*
    PromptFileNum=4;
    PromptLocalEntry=204958;
    PromptTrigNum=993566;
    DelayedFileNum=4;
    DelayedLocalEntry=204959;
    DelayedTrigNum=993567;
	*/
//zhangfh:  ['22380', '56', '56', '1', '112114', '112115', '13461584', '13461585', '3.7', '7.59', '61.4']	
    PromptFileNum=56;
    PromptLocalEntry=112114;
    PromptTrigNum=13461584;
    DelayedFileNum=0;
    DelayedLocalEntry=112115;
    DelayedTrigNum=13461585;
	
}

bool IBDSelSimpAlg::initialize()
{
	m_buffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
  return true;
}

bool IBDSelSimpAlg::finalize()
{
  return true;
}

bool IBDSelSimpAlg::execute()
{
  PhyEvent *event = m_buffer->curEvt();
  //if(event->m_entry%100000==0) cout << "------entry=" << event->m_entry << endl;
  if(event->isMuon()) {
    m_muonVeto->updateVetoWindow(event);
  }

  if(!event->isAD()) {return true; }

  if(!(event->m_det>=1&&event->m_det<=4)) {
    LogError << "Wrong detector." << endl;
  }
  
  if(event->m_flasherTag || event->m_2inchFlasherTag)  { 
    return true; 
  }
  
  double ellipse = (event->m_quadrant*event->m_quadrant) +
                   (event->m_maxQ/0.45)*(event->m_maxQ/0.45);
  if(ellipse>1.) std::cout << "Error! Ellipse > 1 after flasher cut! " << endl;
  
  if(m_muonVeto->veto(event)) {
    return true;
  }

  if(event->isAdMuon() || event->isShowerMuon() ) return true;
  
  if(event->energy() <= m_eplow) { return true; }

  if( event->m_fileNum==PromptFileNum && event->m_localEntry==PromptLocalEntry && event->m_trigNum==PromptTrigNum )
  {
	  std::cout<<" "<<endl;
	  std::cout << "!!!!!!!!!FIND THE PROMPT SIGNAL!!!!!!!!!! " << endl; 
	 print(event);
  }
  if( event->m_fileNum==DelayedFileNum && event->m_localEntry==DelayedLocalEntry && event->m_trigNum==DelayedTrigNum )
  {
	  std::cout<<" "<<endl;
	 std::cout << "!!!!!!!!!FIND THE DELAYED SIGNAL!!!!!!!!!! " << endl; 
	 print(event);
  }
  processEvt(event, m_adBuf[event->m_det-1]);
  m_adBuf[event->m_det-1].push_back(event->GrabInstance());

  return true;
}

void IBDSelSimpAlg::processEvt(PhyEvent* event, std::vector<PhyEvent*>& evtBuf) 
{
  PhyEvent *lastEvt = 0;
  if( (event->m_fileNum==PromptFileNum && event->m_localEntry==PromptLocalEntry && event->m_trigNum==PromptTrigNum)||(event->m_fileNum==DelayedFileNum && event->m_localEntry==DelayedLocalEntry && event->m_trigNum==DelayedTrigNum) )
  {
	  
  if(!evtBuf.empty()) {
	std::cout << " " << endl;
	std::cout << "evtBuf is NOT empty , size of it is  :[" <<evtBuf.size() <<"]"<<endl;
    lastEvt = evtBuf[evtBuf.size()-1];
	for( int i=0 ; i<(int)evtBuf.size() ; i++ )
	{
		std::cout << "follwing is info of the [" <<i+1 <<"th] signal in buffer ."<<endl;
		print(evtBuf[i]);
	}
	
    double t2last = event->m_trigTime - lastEvt->m_trigTime;
    if(t2last > m_dthigh) { // find a time gap, deal with events in buffer
	  std::cout << "event->m_trigTime - lastEvt->m_trigTime > m_dthigh : " <<t2last <<endl;
      fillCoinc(evtBuf,0);
      for(unsigned int i=0;i<evtBuf.size();i++) {
        evtBuf[i]->ReleaseInstance();
      }
	std::cout << " start to clear evtBuf ..." << endl;
	std::cout << "==> CurEvent be the 1th signal in buffer . " << endl;
      evtBuf.clear();
    }else
	{
	  std::cout << "event->m_trigTime - lastEvt->m_trigTime < m_dthigh : " <<t2last <<endl;
	  std::cout << "push_back CurEvent into buffer, be the [ " <<evtBuf.size()+1 <<"th] signal ."<<endl;
	}
  }else
  {
	std::cout << "==> evtBuf is empty() " << endl;
	std::cout << "==> Be the 1th signal in buffer . " << endl;
  }
  }else
  {
  
  if(!evtBuf.empty()) {
    lastEvt = evtBuf[evtBuf.size()-1];
	
    double t2last = event->m_trigTime - lastEvt->m_trigTime;
    if(t2last > m_dthigh) { // find a time gap, deal with events in buffer
      fillCoinc(evtBuf,0);
      for(unsigned int i=0;i<evtBuf.size();i++) {
        evtBuf[i]->ReleaseInstance();
      }
      evtBuf.clear();
    }
  }
  }

}

bool IBDSelSimpAlg::fillCoinc(const vector<PhyEvent*> &evtGroup , bool IsFind) {
	if( (evtGroup[0]->m_fileNum==PromptFileNum && evtGroup[0]->m_localEntry==PromptLocalEntry && evtGroup[0]->m_trigNum==PromptTrigNum)&&(evtGroup[1]->m_fileNum==DelayedFileNum && evtGroup[1]->m_localEntry==DelayedLocalEntry && evtGroup[1]->m_trigNum==DelayedTrigNum) )
	{
		IsFind=1;
	}
	if( IsFind )
	{
		std::cout<<"||"<<endl;
		std::cout<<"||"<<endl;
		std::cout<<"\\/"<<endl;
		std::cout<<"Begin to execute the pairs . "<<endl;
	}
	m_Nevt = evtGroup.size();
  m_det = evtGroup[0]->m_det;

  // If no neutron-like trigger inside the cluster, rejected
  int hasHEevt = 0;
  for(int i=0; i<m_Nevt; i++) {
    if(evtGroup[i]->energy()>m_edlow)  hasHEevt = 1; 
  }
  if(hasHEevt==0) return true; 

  // IBD selection
  m_isVetoed=0;
  if(m_Nevt==2 && m_isVetoed==0) {
	  if( IsFind )
	  {
	  std::cout<<"==>"<<endl;
	  std::cout<<"==> Begin to select IBD ."<<endl;
	  }
	  for( int i=0 ; i<2 ; i++ )
	  {
		  calcT2Mus(evtGroup[i]);
		  m_t2preIWSMuon[i] = m_t2Mus[0];
		  m_t2nextIWSMuon[i] = m_t2Mus[1];
		  m_t2preOWSMuon[i] = m_t2Mus[2];
		  m_t2nextOWSMuon[i] = m_t2Mus[3];
		  m_t2preRPCMuon[i] = m_t2Mus[4];
		  m_t2nextRPCMuon[i] = m_t2Mus[5];
		  m_t2nonShowerMuon[i] = m_t2Mus[6];
		  m_t2showerMuon[i] = m_t2Mus[7];
		  m_t2nextNonShowerMuon[i] = m_t2Mus[8];
		  if( IsFind )
		  {
			std::cout<<"m_t2preIWSMuon ["<<i<<"] : "<<m_t2preIWSMuon[i] <<endl;
			//std::cout<<"m_t2nextIWSMuon ["<<i<<"] : "<<m_t2nextIWSMuon[i] <<endl;
			std::cout<<"m_t2preOWSMuon ["<<i<<"] : "<<m_t2preOWSMuon[i] <<endl;
			//std::cout<<"m_t2nextOWSMuon ["<<i<<"] : "<<m_t2nextOWSMuon[i] <<endl;
			std::cout<<"m_t2preRPCMuon ["<<i<<"] : "<<m_t2preRPCMuon[i] <<endl;
			//std::cout<<"m_t2nextRPCMuon ["<<i<<"] : "<<m_t2nextRPCMuon[i] <<endl;
			std::cout<<"m_t2nonShowerMuon ["<<i<<"] : "<<m_t2nonShowerMuon[i] <<endl;
			std::cout<<"m_t2showerMuon ["<<i<<"] : "<<m_t2showerMuon[i] <<endl;
			//std::cout<<"m_t2nextNonShowerMuon ["<<i<<"] : "<<m_t2nextNonShowerMuon[i] <<endl;
		  }
	  }
	  
    m_Energy[0] = evtGroup[0]->energy();
    m_Energy[1] = evtGroup[1]->energy();
    m_deltaT[1] = evtGroup[1]->m_trigTime - evtGroup[0]->m_trigTime;

	if( IsFind )
	{
		
    if(m_Energy[0]>m_eplow && m_Energy[1]>m_edlow && m_Energy[1]<m_edhigh && m_deltaT[1]>m_dtlow) {
      m_isIbd=0;
      if(m_Energy[0]<m_ephigh) {
        if(m_t2preIWSMuon[1]>m_muVetoWin && m_t2nextIWSMuon[1]>m_preVetoWin) {
          if(m_t2preOWSMuon[1]>m_muVetoWin && m_t2nextOWSMuon[1]>m_preVetoWin) {
            if(m_t2nonShowerMuon[1]>m_adMuVetoWin && m_t2showerMuon[1]>m_showerMuVetoWin ) {
              std::cout << ">>>>> find a IBD . This is what I look for .  <<<<< " << endl;
				m_isIbd=1;
            }else
			{
				std::cout<<"m_t2nonShowerMuon[1]>m_adMuVetoWin && m_t2showerMuon[1]>m_showerMuVetoWin ==> is false . "<<endl;
				std::cout<<"  m_t2nonShowerMuon[1] : "<<m_t2nonShowerMuon[1] <<endl;
				std::cout<<"  m_adMuVetoWin : "<<m_adMuVetoWin <<endl;
				std::cout<<"  m_t2showerMuon[1] : "<< m_t2showerMuon[1]<<endl;
				std::cout<<"  m_showerMuVetoWin : "<<m_showerMuVetoWin <<endl;
			}
          }else
		  {
			std::cout<<"m_t2preOWSMuon[1]>m_muVetoWin && m_t2nextOWSMuon[1]>m_preVetoWin ==> is false . "<<endl;
			std::cout<<"  m_t2preOWSMuon[1] : "<<m_t2preOWSMuon[1] <<endl;
			std::cout<<"  m_muVetoWin : "<<m_muVetoWin <<endl;
			std::cout<<"  m_t2nextOWSMuon[1] : "<<m_t2nextOWSMuon[1] <<endl;
			std::cout<<"  m_preVetoWin : "<<m_preVetoWin <<endl;
		  }
        }else
		{
			std::cout<<"m_t2preIWSMuon[1]>m_muVetoWin && m_t2nextIWSMuon[1]>m_preVetoWin ==>is false ."<<endl;
			std::cout<<"  m_t2preIWSMuon[1] : "<<m_t2preIWSMuon[1] <<endl;
			std::cout<<"  m_muVetoWin : "<<m_muVetoWin <<endl;
			std::cout<<"  m_t2nextIWSMuon[1] : "<<m_t2nextIWSMuon[1] <<endl;
			std::cout<<"  m_preVetoWin : "<<m_preVetoWin <<endl;
		}
      }else
	  {
		std::cout<<"m_Energy[0]<m_ephigh ==> is false . "<<endl;
		std::cout<<"  m_Energy[0] : "<<m_Energy[0] <<endl;
		std::cout<<"  m_ephigh : "<<m_ephigh <<endl;
	  }
    }else
	{
		std::cout<<"m_Energy[0]>m_eplow && m_Energy[1]>m_edlow && m_Energy[1]<m_edhigh && m_deltaT[1]>m_dtlow ==> is false ."<<endl;
		std::cout<<"  m_Energy[0] : "<<m_Energy[0] <<endl;
		std::cout<<"  m_eplow : "<<m_eplow <<endl;
		std::cout<<"  m_Energy[1] : "<<m_Energy[1] <<endl;
		std::cout<<"  m_edlow : "<<m_edlow <<endl;
		std::cout<<"  m_edhigh : "<<m_edhigh <<endl;
		std::cout<<"  m_deltaT[1] : "<<m_deltaT[1] <<endl;
		std::cout<<"  m_dtlow : "<<m_dtlow <<endl;
	}
		std::cout<<"finish the work ! "<<endl;
		std::cout<<"/\\"<<endl;
		std::cout<<"||"<<endl;
		std::cout<<"||"<<endl;
		std::cout<<""<<endl;
	} else
	{
    if(m_Energy[0]>m_eplow && m_Energy[1]>m_edlow && m_Energy[1]<m_edhigh && m_deltaT[1]>m_dtlow) {
      m_isIbd=0;
      if(m_Energy[0]<m_ephigh) {
        if(m_t2preIWSMuon[1]>m_muVetoWin && m_t2nextIWSMuon[1]>m_preVetoWin) {
          if(m_t2preOWSMuon[1]>m_muVetoWin && m_t2nextOWSMuon[1]>m_preVetoWin) {
            if(m_t2nonShowerMuon[1]>m_adMuVetoWin && m_t2showerMuon[1]>m_showerMuVetoWin ) {
              //std::cout << ">>>>> find a IBD <<<<< " << endl;
				m_isIbd=1;
            }
          }
        }
      }
    }
		
	}

	}
  return true;
}

void IBDSelSimpAlg::print(PhyEvent* event)
{
  std::cout << "------entry=" << event->m_entry
    << ", " << event
    << ", fileNum=" << event->m_fileNum
    << ", local entry=" << event->m_localEntry
    << ", trigtime=" << event->m_trigTime
    << ", trigType=" << event->m_trigType
    << ", det=" << event->m_det 
    << ", energy=" << event->m_energy
    << ", rawEvis=" << event->m_rawEvis
    << ", energy()=" << event->energy()
    << ", nPmt=" << event->m_nPmt
    << ", flasherTag=" << event->m_flasherTag 
    << ", forceTrigTag=" << event->m_forceTrigTag
    << ", crossTrigTag=" << event->m_crossTrigTag
    << ", rpcNoiseTag=" << event->m_rpcNoiseTag
    << ", adLowEnergyTag=" << event->m_adLowEnergyTag
    << ", goodevent=" << event->m_good
    << ", muontag=" << event->m_muonTag
    << endl;
}

bool IBDSelSimpAlg::calcT2Mus(PhyEvent* event) {
	  // Time to IWS muon
	  PhyEvent *preMu = m_muonVeto->preMuon(event, 5);
    if(preMu) {
	  m_t2Mus[0] = event->m_trigTime - preMu->m_trigTime;
    }
	PhyEvent *nextMu = m_muonVeto->nextMuon(event, 5);
	if(nextMu) {
	  m_t2Mus[1] = nextMu->m_trigTime - event->m_trigTime;
	} 
	  // Time to OWS muon
	preMu = m_muonVeto->preMuon(event, 6);
	if(preMu) {
	  m_t2Mus[2] = event->m_trigTime - preMu->m_trigTime;
	}
	nextMu = m_muonVeto->nextMuon(event, 6);
	if(nextMu) {
	   m_t2Mus[3] = nextMu->m_trigTime - event->m_trigTime;
	}
	// Time to AD Muons
	preMu = m_muonVeto->preMuon(event, event->m_det);
	if(preMu) {
	   m_t2Mus[6] = event->m_trigTime - preMu->m_trigTime;
	}
    preMu = m_muonVeto->preShowerMuon(event);
    if(preMu) {
       m_t2Mus[7] = event->m_trigTime - preMu->m_trigTime;
    }
    nextMu = m_muonVeto->nextMuon(event, event->m_det);
    if(nextMu) {
      m_t2Mus[8] = nextMu->m_trigTime - event->m_trigTime;
    }

    return true;
}
