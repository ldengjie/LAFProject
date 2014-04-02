#include "Algorithms/MichelAlg.h"
#include "LafKernel/GlobalVar.h"
#include <iostream>

using namespace std;

MichelAlg::MichelAlg(const std::string& name)
  : AlgBase(name)
{
  setOption("MuEngLow",  m_muEngLow  = 80.);  //80MeV
  setOption("MuEngHigh", m_muEngHigh = 1000.);  //1GeV
  setOption("T2MuLow",   m_t2muLow   = 2.e-6);  //2us
  setOption("T2MuHigh",  m_t2muHigh  = 1.e-5);  //10us

  for(int i=0; i<GlobalVar::NumADs; i++) {
    m_muEng[i] = 0.;
    m_muTime[i] = 0;
  }
}

bool MichelAlg::initialize()
{
  LogDebug << "MuEngLow = " << m_muEngLow << std::endl;
  m_buffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
  m_tuple = ntupleSvc()->bookNtuple("FILE1/Michel", "Michel", "det:energy:muenergy:t2mu");

  return true;
}

bool MichelAlg::finalize()
{
  LogDebug << "---MichelAlg::finalize()---" << endl;
  return true;
}

bool MichelAlg::execute()
{

  // If bad event, return
  PhyEvent *event = m_buffer->curEvt();
  if ( !event->m_good ) return true;
  if ( !event->isAD() ) return true;

  // tag muon
  if(event->m_energy>m_muEngLow && event->m_energy<m_muEngHigh) { // find a muon
    m_muEng[event->m_det-1] = event->m_energy;
    m_muTime[event->m_det-1] = event->m_trigTime;
    return true;
  }

  double t2mu = (event->m_trigTime - m_muTime[event->m_det-1]);
  if(t2mu>m_t2muLow && t2mu<m_t2muHigh) {
      LogDebug << event->m_det << " " << event->m_energy
	       << " " << m_muEng[event->m_det-1] << " " << t2mu << endl;
      m_tuple->Fill(event->m_det,event->m_energy,m_muEng[event->m_det-1],t2mu);
  }

  return true;
}
