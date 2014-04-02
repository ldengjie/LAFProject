#include "DMCIBDSelectAlg.h"
#include "MuonVeto/MuonVeto.h"
#include "LiveTimeSvc/LiveTimeSvc.h"
#include "LafKernel/AlgFactory.h"
#include "LafKernel/GlobalVar.h"
#include <iostream>
#include <TTree.h>
#include <math.h>

using namespace std;

DECLARE_ALGORITHM(DMCIBDSelectAlg);

DMCIBDSelectAlg::DMCIBDSelectAlg(const std::string& name)
: AlgBase(name)
{
  setOption("PromptEngLow",m_eplow);  
  setOption("PromptEngHigh",m_ephigh);  
  setOption("DelayedEngLow",m_edlow);  
  setOption("DelayedEngHigh",m_edhigh);  
  setOption("DtLow",m_dtlow);  
  setOption("DtHigh",m_dthigh);  

  for(int det=1;det<=GlobalVar::NumADs;det++) {
    m_adBuf[det-1].clear();    
  }
}

bool DMCIBDSelectAlg::initialize()
{
  m_buffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));

  m_tree = new TTree("dmc","dmc");
  m_tree->Branch("ep",&m_tep,"ep/D");
  m_tree->Branch("xp",&m_txp,"xp/D");
  m_tree->Branch("yp",&m_typ,"yp/D");
  m_tree->Branch("zp",&m_tzp,"zp/D");
  m_tree->Branch("en",&m_ten,"en/D");
  m_tree->Branch("xn",&m_txn,"xn/D");
  m_tree->Branch("yn",&m_tyn,"yn/D");
  m_tree->Branch("zn",&m_tzn,"zn/D");
  m_tree->Branch("dt",&m_tdt,"dt/D");
  m_tree->Branch("det",&m_tdet,"det/I");
  m_tree->Branch("pfile",&m_tpfile,"pfile/I");
  m_tree->Branch("pentry",&m_tpentry,"pentry/I");
  m_tree->Branch("nfile",&m_tnfile,"nfile/I");
  m_tree->Branch("nentry",&m_tnentry,"nentry/I");
  m_tree->Branch("ptrignum",&m_tptrignum,"ptrignum/I");
  m_tree->Branch("ntrignum",&m_tntrignum,"ntrignum/I");
  m_tree->Branch("tn",&m_tn,"tn/I");
  ntupleSvc()->attach("FILE1/hem/tree/",m_tree);

  return true;
}

bool DMCIBDSelectAlg::finalize()
{
  return true;
}

bool DMCIBDSelectAlg::execute()
{

  // Get current event
  PhyEvent *event = m_buffer->curEvt();
  //event->print();

  LogDebug << "------entry=" << event->m_entry
    << ", fileNum=" << event->m_fileNum
    << ", local entry=" << event->m_localEntry
    << ", trigNum=" << event->m_trigNum
    << ", trigtime=" << event->m_trigTime
    << ", det=" << event->m_det 
    << ", energy=" << event->energy()
    << ", flasherTag=" << event->m_flasherTag 
    << ", 2inchFlasherTag=" << event->m_2inchFlasherTag
    << ", forceTrigTag=" << event->m_forceTrigTag
    << ", crossTrigTag=" << event->m_crossTrigTag
    << ", rpcNoiseTag=" << event->m_rpcNoiseTag
    << ", adLowEnergyTag=" << event->m_adLowEnergyTag
    << ", goodevent=" << event->m_good
    << ", muontag=" << event->m_muonTag
    << endl;

  LogDebug << "Print event in buffer." << endl;
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    LogDebug << "Det: " << det << endl;
    for(unsigned int j=0;j<m_adBuf[det-1].size();j++) {
      LogDebug << j << "," << m_adBuf[det-1][j] << "," 
        << m_adBuf[det-1][j]->m_det << "," << m_adBuf[det-1][j]->m_entry << endl;
    }
  }

  if(!event->isAD()) {
    LogDebug << "Skip none AD event." << endl;
    return true;
  }

  if(event->m_flasherTag || event->m_2inchFlasherTag 
      || event->m_forceTrigTag || event->m_crossTrigTag) {
    LogDebug << "Skip bad event." << endl;
    return true;
  }

  if(event->isMuon()) {
    LogDebug << "Skip muon event." << endl;
    return true;
  }

  // energy cut
  if(!(event->energy()>m_eplow && event->energy()<m_ephigh)) {
    LogDebug << "Out of energy range (0.7MeV,12MeV)." << endl;
    return true;
  }

  // Make a user tag if this event is in muon veto window
  event->m_userTag["MuonVeto"] = 0.;
  MuonVeto *muonVeto = MuonVeto::instance();
  if(muonVeto->veto(event) && event->energy()>m_edlow && event->energy()<m_edhigh) {
    LogDebug << "This neutron like signal is in muon veto window." << endl;
    event->m_userTag["MuonVeto"] = 1.;
  }

  // Multi-coincidence selection
  PhyEvent *lastEvt = 0;
  if(!m_adBuf[event->m_det-1].empty()) {
    lastEvt = m_adBuf[event->m_det-1][m_adBuf[event->m_det-1].size()-1];
  } else {
    LogDebug << "push_back: " << event->m_entry << endl;
    LogDebug << "Empty buffer. Either this is the first event or something wrong." << endl;
    // GrabInstance() is used as a smart pointer, 
    // to make sure event in local buffer will not be deleted,
    // even if it's out of the buffer time window
    m_adBuf[event->m_det-1].push_back(event->GrabInstance());
    return true;
  }

  if(lastEvt) {
    double t2last = event->m_trigTime - lastEvt->m_trigTime;
    if(t2last > 2*m_dthigh) { // find a >400us time gap, deal with events in buffer
      fillIbd(m_adBuf[event->m_det-1]);
      // Release the smart pointer when this event is removed from local buffer,
      // to make sure no memory leak
      for(unsigned int i=0;i<m_adBuf[event->m_det-1].size();i++) {
        m_adBuf[event->m_det-1][i]->ReleaseInstance();
      }
      m_adBuf[event->m_det-1].clear();
    } 
    LogDebug << "push_back: " << event->m_entry << endl;
    m_adBuf[event->m_det-1].push_back(event->GrabInstance());
  } else {
    LogError << "Can't find last event. Check your code." << endl;
  }

  return true;
}

bool DMCIBDSelectAlg::fillIbd(const vector<PhyEvent*> &evtGroup) {

  LogDebug << "Multiplicity of event group: " << evtGroup.size() << endl;

  // for each neutron-like event, find "e+"-like coincidence
  for(unsigned i=1;i<evtGroup.size();i++) { // the first event can't be neutron

    // Find neutron-like event
    if(evtGroup[i]->energy()<=m_edlow) {
      LogDebug << "This is not a neutron(E<6MeV)." << endl;
      continue;
    }

    // Apply muon veto on neutron-like event
    if(evtGroup[i]->m_userTag["MuonVeto"]>0) {
      LogDebug << "Veto delayed signal by muon." << endl;
      continue;
    }

    // Time interval cut: 1us~200us
    double dt = evtGroup[i]->m_trigTime - evtGroup[i-1]->m_trigTime;
    if(dt<=m_dtlow || dt>=m_dthigh) {
      LogDebug << "Time interval cut(1us~200us) is not satisfied: " 
        << dt*1.e6 << " us" << endl;
      continue;
    }

    // No more "e+"-like event before neutron in 400us window
    if(i>=2) {
      dt = evtGroup[i]->m_trigTime - evtGroup[i-2]->m_trigTime;
      if(dt<=2*m_dthigh) {
        LogDebug << "Find another prompt signal in previous 400us window: "
          << dt*1.e6 << " us" << endl;
        continue;
      }
    }

    // No neutron-like event after neutron in 200us window
    // If this neutron-like event is vetoed by muon, it will be ignored.
    if(i<evtGroup.size()-1) {
      if(evtGroup[i+1]->energy()>m_edlow && !(evtGroup[i+1]->m_userTag["MuonVeto"]>0)) {
        dt = evtGroup[i+1]->m_trigTime - evtGroup[i]->m_trigTime;
        if(dt<=m_dthigh) {
          LogDebug << "Find another delayed signal in following 200us window: "
            << dt*1.e6 << " us" << endl;
          continue;
        }
      }
    }

    // Find an IBD candidate
    PhyEvent *dEvt = evtGroup[i];
    PhyEvent *pEvt = evtGroup[i-1];
    dt = (dEvt->m_trigTime - pEvt->m_trigTime)*1.e6;

    m_tep = pEvt->energy();
    m_txp = pEvt->m_x;
    m_typ = pEvt->m_y;
    m_tzp = pEvt->m_z;
    m_ten = dEvt->energy();
    m_txn = dEvt->m_x;
    m_tyn = dEvt->m_y;
    m_tzn = dEvt->m_z;
    m_tdt = dt;
    m_tdet = dEvt->m_det;
    m_tpfile = pEvt->m_fileNum;
    m_tpentry = pEvt->m_localEntry;
    m_tnfile = dEvt->m_fileNum;
    m_tnentry = dEvt->m_localEntry;
    m_tptrignum = pEvt->m_trigNum;
    m_tntrignum = dEvt->m_trigNum;
    m_tn = dEvt->m_trigTime.GetSec();
    m_tree->Fill();

    LogInfo << "Find an IBD candidate in file " << pEvt->m_fileNum
      << ", local entry " << pEvt->m_localEntry << ", trigNum " << pEvt->m_trigNum
      << ", and file " << dEvt->m_fileNum
      << ", local entry " << dEvt->m_localEntry << ", trigNum " << dEvt->m_trigNum
      << endl;
    LogInfo << "Find an IBD candidate with (det, ep, ed, dt) = ("
      << pEvt->m_det << "," << pEvt->energy() << "," << dEvt->energy()
      << "," << dt << ")" << endl;
  }

  return true;
}
