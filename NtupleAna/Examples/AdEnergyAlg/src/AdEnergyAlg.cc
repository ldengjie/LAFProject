#include "AdEnergyAlg.h"
#include "MuonVeto/MuonVeto.h"
#include "LafKernel/AlgFactory.h"
#include <iostream>

using namespace std;

DECLARE_ALGORITHM(AdEnergyAlg);

AdEnergyAlg::AdEnergyAlg(const std::string& name)
: AlgBase(name)
{
}

bool AdEnergyAlg::initialize()
{
    m_buffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
    m_tuple = ntupleSvc()->bookNtuple("FILE1/AdEnergy", "AdEnergy", "det:energy:x:y:z");
    return true;
}

bool AdEnergyAlg::finalize()
{
  return true;
}

bool AdEnergyAlg::execute()
{

  // If bad event, return
  PhyEvent *event = m_buffer->curEvt();
  //if(!event->m_good) return true;
  if(!event->isAD()) return true;

  // If in muon veto window, return
  MuonVeto *muonVeto = MuonVeto::instance();
  if(muonVeto->veto(event)) return true;
  
  //cout << *event;
  m_tuple->Fill(event->m_det,event->m_energy,event->m_x,event->m_y,event->m_z);

  return true;
}
