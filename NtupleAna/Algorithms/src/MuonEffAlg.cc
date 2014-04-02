#include "Algorithms/MuonEffAlg.h"
#include "LafKernel/GlobalVar.h"
#include <iostream>

using namespace std;

MuonEffAlg::MuonEffAlg(const std::string& name)
  : AlgBase(name)
{
  m_search_twd = 600.; //ns
  m_eBinNum = 4;  // energy bin number
  m_eStep = 100.; // energy step
  for(int i=0;i<GlobalVar::NumADs;i++) {
    for(int j=0;j<m_eBinNum;j++) {
      m_adNum[i][j] = 0;
      m_iwsNum[i][j] = 0;
      m_owsNum[i][j] = 0;
    }
  }
  for(int j=0;j<m_eBinNum;j++) {
    m_energyCut[j] = m_eStep*(j+1);
  }
}

bool MuonEffAlg::initialize()
{
    m_buffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
    m_tuple = ntupleSvc()->bookNtuple("FILE1/MuonEff", "MuonEff", "det:energy:x:y:z");
    return true;
}

bool MuonEffAlg::finalize()
{
  LogDebug << "---MuonEffAlg::finalize()---" << endl;
  for(int j=0;j<m_eBinNum;j++) {
    LogDebug << "Muon energy cut: " << m_energyCut[j] << endl;
    for(int i=0;i<GlobalVar::NumADs;i++) {
      LogDebug << "AD" << i+1 << " muon event number = " << m_adNum[i][j] << endl;
      LogDebug << "IWS observe number = " << m_iwsNum[i][j] << endl;
      LogDebug << "OWS observe number = " << m_owsNum[i][j] << endl;
    }
  }
  return true;
}

bool MuonEffAlg::execute()
{

  // If bad event, return
  PhyEvent *event = m_buffer->curEvt();
  //if(!event->m_good) return true;
  if(!event->isAD()) return true;

  for(int j=0;j<m_eBinNum;j++) {
    if(event->m_energy > m_energyCut[j]) {  //find a muon
      m_adNum[event->m_det-1][j]++;

      //find IWS and OWS readout in a time window before AD
      PhyEventBuf::Iterator evt = m_buffer->current();
      while ( (--evt) >= m_buffer->begin() ) {
        if((event->m_trigTime-(*evt)->m_trigTime)*1e9 < m_search_twd) {
          if((*evt)->m_det==5) m_iwsNum[event->m_det-1][j]++;
          else if((*evt)->m_det==6) m_owsNum[event->m_det-1][j]++;
        } else {
          break;
        }
      }

      //find IWS and OWS readout in a time window after AD
      evt = m_buffer->current();
      while ( (++evt) < m_buffer->end() ) {
        if(((*evt)->m_trigTime-event->m_trigTime)*1e9 < m_search_twd) {
          if((*evt)->m_det==5) m_iwsNum[event->m_det-1][j]++;
          else if((*evt)->m_det==6) m_owsNum[event->m_det-1][j]++;
        } else {
          break;
        }
      }

    }
  }

  //cout << *event;
  m_tuple->Fill(event->m_det,event->m_energy,event->m_x,event->m_y,event->m_z);

  return true;
}
