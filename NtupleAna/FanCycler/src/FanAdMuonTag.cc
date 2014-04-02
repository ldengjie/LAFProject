#include "FanAdMuonTag.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/ToolFactory.h"

DECLARE_TOOL(FanAdMuonTag);

FanAdMuonTag::FanAdMuonTag(const std::string& name)
    : IFanTagBase(),
      ToolBase(name)
{
    setOption("ApplyVeto", m_veto=false);
    setOption("ApplySatCor", m_cor=false);
    setOption("AdMuonCut", m_adMuonCut=false);

    setOption("AdMuonThreshold", m_adMuonThr);
    setOption("ShowerMuonThreshold", m_showerMuonThr);
    setOption("CoincWindow", m_window);
    if ( m_window.size() != 2 ) {
	m_window.clear();
	m_window.push_back(-600e-9);
	m_window.push_back(600e-9);
    }
}

bool FanAdMuonTag::tag(PhyEventBuf *buf)
{
    // Tagging the current event in buf
    PhyEvent* event = buf->curEvt();

    if( m_cor && event->m_det < 5 && event->energy() > (m_adMuonThr-3))
    {
        CalibReadout* calib = event->get<CalibReadout>("Event/CalibReadout/CalibReadoutHeader");
        double chargeADtotal=0.;
        double chargeADtotalAfterCor=0.;
        double chargeADAfterCor=0.;
        for( unsigned int i=0 ; i<calib->nHitsAD ; i++ )
        {
            /*
            if( calib->ring.at(i)>8 || calib->column.at(i)>24 )
            {
                std::cout<<"wrong "<<endl;
            }
            */
            if( calib->hitCountAD.at(i)==0 )
            {
                chargeADtotal+=calib->chargeAD.at(i);
                chargeADAfterCor=calib->chargeAD.at(i)*TMath::Sqrt(2*calib->chargeAD.at(i)*calib->chargeAD.at(i)/(1000000)+1);
                chargeADtotalAfterCor+=chargeADAfterCor;
            }
        }
        double EnergyRecCoefficient=chargeADtotal/event->m_rawEvis;
        double HeterogeneityCoefficient=event->m_enrec/event->m_rawEvis;

        double EnergyAfterSatCor=chargeADtotalAfterCor/EnergyRecCoefficient;
        double EnergyAfterSatAndHetCor=EnergyAfterSatCor*HeterogeneityCoefficient;

        event->setEnergy(EnergyAfterSatAndHetCor);
    }

    if ( event->m_det < 5 ) {
		if ( event->energy() > m_adMuonThr ) {
		  PhyEventBuf::Iterator it = buf->current();
		  while ( --it >= buf->begin() ) {
		    PhyEvent* _evt = *it;
		    if ( (_evt->m_trigTime - event->m_trigTime) > m_window[0] )
	        {
			    if ( !_evt->isAD() && _evt->isWpMuon() ) {
			      if ( event->energy() <= m_showerMuonThr ) {
                      if( m_adMuonCut && event->energy()<60. )
                      {
                          PhyEventBuf::Iterator iit=buf->current();
                          while(  --iit >=buf->begin() && (event->m_trigTime-(*iit)->m_trigTime<1.e-5))
                          {
                              if( (*iit)->isAdMuon() )
                              {
                                return false;    
                              }
                          }
                      }
				      event->m_muonTag = PhyEvent::kAdMuon;
				      return m_veto;
			      }
			      else {
				        event->m_muonTag = PhyEvent::kShowerMuon;
				        return m_veto;
			      }
			    }
	        }
		  }
		  it = buf->current();
		  while ( ++it < buf->end() ) {
		    PhyEvent* _evt = *it;
		    if ( (_evt->m_trigTime - event->m_trigTime) < m_window[1] ) 
	        {
			    if ( !_evt->isAD() && _evt->isWpMuon() ) {
			      if ( event->energy() <= m_showerMuonThr ) {
                      if( m_adMuonCut && event->energy()<60. )
                      {
                          PhyEventBuf::Iterator iit=buf->current();
                          while(  --iit >=buf->begin() && (event->m_trigTime-(*iit)->m_trigTime<1.e-5))
                          {
                              if( (*iit)->isAdMuon() )
                              {
                                return false;    
                              }
                          }
                      }
				    event->m_muonTag = PhyEvent::kAdMuon;
				    return m_veto;
			      }
			      else {
				event->m_muonTag = PhyEvent::kShowerMuon;
				return m_veto;
			      }
			    }
	        }
		  }
		  it = buf->current();
		  while ( --it >= buf->begin() ) {
		    PhyEvent* _evt = *it;
		    if ( (_evt->m_trigTime - event->m_trigTime) > m_window[0] )
	        {
			    if ( !_evt->isAD() && _evt->isRpcMuon() ) {
			      if ( event->energy() <= m_showerMuonThr ) {
				event->m_muonTag = PhyEvent::kAdMuonNew;
				return m_veto;
			      }
			      else {
				event->m_muonTag = PhyEvent::kShowerMuonNew;
				return m_veto;
			      }
			    }
	        }
		  }
		  it = buf->current();
		  while ( ++it < buf->end() ) {
		    PhyEvent* _evt = *it;
		    if ( (_evt->m_trigTime - event->m_trigTime) < m_window[1] )
	        {
			    if ( !_evt->isAD() && _evt->isRpcMuon() ) {
			      if ( event->energy() <= m_showerMuonThr ) {
				event->m_muonTag = PhyEvent::kAdMuonNew;
				return m_veto;
			      }
			      else {
				event->m_muonTag = PhyEvent::kShowerMuonNew;
				return m_veto;
			      }
			    }
	        }
		  }
		}
    }

    return false;
}
