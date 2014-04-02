#include "IBDSelectAlg.h"
#include "MuonVeto/MuonVeto.h"
#include "LiveTimeSvc/LiveTimeSvc.h"
#include "LafKernel/AlgFactory.h"
#include "LafKernel/GlobalVar.h"
#include <iostream>
#include <sstream>
#include <TH1F.h>
#include <TTree.h>
#include <math.h>

using namespace std;

DECLARE_ALGORITHM(IBDSelectAlg);

IBDSelectAlg::IBDSelectAlg(const std::string& name)
: AlgBase(name)
{
  setOption("SaveTree",m_saveTree);  
  setOption("PromptEngLow",m_eplow);  
  setOption("PromptEngHigh",m_ephigh);  
  setOption("DelayedEngLow",m_edlow);  
  setOption("DelayedEngHigh",m_edhigh);  
  setOption("DtLow",m_dtlow);  
  setOption("DtHigh",m_dthigh);  

  for(int det=1;det<=GlobalVar::NumADs;det++) {
    m_adBuf[det-1].clear();    
    m_nibd[det-1] = 0;
    m_npsingles[det-1] = 0;
    m_nnsingles[det-1] = 0;
  }
}

bool IBDSelectAlg::initialize()
{
  LogInfo << "initialize()" << endl;
  m_buffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
  m_liveTimeSvc = dynamic_cast<LiveTimeSvc*>(service("LiveTimeSvc"));
  if(!m_liveTimeSvc) {
    LogError << "Can't get LiveTimeSvc." << endl;
    return false;
  }
  m_muonVeto = MuonVeto::instance();

  for(int det=1;det<=GlobalVar::NumADs;det++) {
    ostringstream sibd;
    sibd << "AD" << det << "IbdNum";
    m_ibdNum[det-1] = new TH1F(sibd.str().c_str(),"IBD number",
        m_liveTimeSvc->nBins(),
        m_liveTimeSvc->startTime().AsDouble(),
        m_liveTimeSvc->endTime().AsDouble()
        );
    ostringstream sps,sns;
    sps << "AD" << det << "ps";
    sns << "AD" << det << "ns";
    m_hPSingles[det-1] = new TH1F(sps.str().c_str(),"Prompt like singles",100,m_eplow,m_ephigh);
    m_hNSingles[det-1] = new TH1F(sns.str().c_str(),"Neutron like singles",100,m_edlow,m_edhigh);
    ntupleSvc()->attach("FILE1/hem/IBD/",m_ibdNum[det-1]);
    ntupleSvc()->attach("FILE1/hem/Singles/",m_hPSingles[det-1]);
    ntupleSvc()->attach("FILE1/hem/Singles/",m_hNSingles[det-1]);
  }

  if(m_saveTree) {
    m_tree = new TTree("ibd","ibd");
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
  }

  return true;
}

bool IBDSelectAlg::finalize()
{
  LogInfo << "finalize()" << endl;
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    LogInfo << "IBD candidates in AD " << det << ": " << m_nibd[det-1] << endl;
    LogInfo << "Prompt-like singles in AD " << det << ": " << m_npsingles[det-1] 
      << " (" << m_npsingles[det-1]/m_liveTimeSvc->hLiveTime(det)->Integral() << " Hz)" << endl;
    LogInfo << "Neutron-like singles in AD " << det << ": " << m_nnsingles[det-1]
      << ", (" << 86400.*m_nnsingles[det-1]/m_liveTimeSvc->hLiveTime(det)->Integral() << " /day)" << endl;
  }
  return true;
}

bool IBDSelectAlg::execute()
{

  // Get current event
  PhyEvent *event = m_buffer->curEvt();
  //event->print();

  LogDebug << "------entry=" << event->m_entry
    << ", " << event
    << ", fileNum=" << event->m_fileNum
    << ", local entry=" << event->m_localEntry
    << ", trigtime=" << event->m_trigTime
    << ", trigNum=" << event->m_trigNum
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

  if(!event->m_good) {
    // 5 types of events are defined as "bad" by default: 
    // flasher, force trigger, cross trigger, RPC 2/4, AD energy < 0.7MeV
    // you can use following equivalent statement:
    //if(event->m_flasherTag || event->m_2inchFlasherTag
    //    || event->energy < m_eplow || event->m_rpcNoiseTag
    //    || event->m_forceTrigTag || event->m_crossTrigTag) 
    LogDebug << "Skip bad event." << endl;
    return true;
  }

  if(event->isMuon()) {
    LogDebug << "Skip muon event." << endl;
    return true;
  }

  // Veto [-2us,200us] for WP muon
  DataBuffer<PhyEvent>::Iterator it=m_buffer->find(event);
  it--;
  while ( it >= m_buffer->begin() ) {
    PhyEvent* evt = (*it);
    double dt = event->m_trigTime - evt->m_trigTime;
    if(dt>2.e-4) break;
    if(evt->isMuon() && (evt->m_det==5 || evt->m_det==6)) {
      if(dt <= 2.e-4) {
        LogDebug << "Veto this event by WP muon in the first step." << endl;
        return true;
      }
    }
    it--;
  }

  it=m_buffer->find(event);
  it++;
  while ( it < m_buffer->end() ) {
    PhyEvent* evt = (*it);
    double dt = evt->m_trigTime - event->m_trigTime;
    if(dt>2.e-6) break;
    if(evt->isMuon() && (evt->m_det==5 || evt->m_det==6)) {
      if(dt <= 2.e-6) {
        LogDebug << "Veto this event by WP muon in the first step." << endl;
        return true;
      }
    }
    it++;
  }

  // Make a user tag if this event is vetoed by 
  // WP muon: [-2us,600us]
  // AD muon: [0,1ms]
  // AD shower muon: [0,1s]
  event->m_userTag["MuonVeto"] = 0.;
  if(m_muonVeto->veto(event)) {
    LogDebug << "This event is in muon veto window." << endl;
    event->m_userTag["MuonVeto"] = 1.;
  }

  // Multi-coincidence(event group) selection
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
    if(t2last > m_dthigh) { // find a time gap, deal with events in buffer
      if(m_adBuf[event->m_det-1].size()==2) {
        fillIbd(m_adBuf[event->m_det-1]);
      } else if(m_adBuf[event->m_det-1].size()==1) {
        fillSingles(m_adBuf[event->m_det-1]);
      }
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

bool IBDSelectAlg::fillIbd(const vector<PhyEvent*> &evtGroup) {
  LogDebug << "Multiplicity of event group: " << evtGroup.size() << endl;

  PhyEvent *pEvt = evtGroup[0]; // prompt
  PhyEvent *dEvt = evtGroup[1]; // delayed

  //Apply muon veto on delayed signal
  if(dEvt->m_userTag["MuonVeto"]>0) {
    LogDebug << "Veto delayed signal by muon." << endl;
    return true;
  }

  // energy cut
  if(pEvt->energy()>m_eplow && pEvt->energy()<m_ephigh 
      && dEvt->energy()>m_edlow && dEvt->energy()<m_edhigh) {
    double timeInt = (dEvt->m_trigTime - pEvt->m_trigTime);
    // time interval cut
    if(timeInt < m_dtlow) {
      LogDebug << "Time interval is too small." << endl;
      return true;
    }

    timeInt *= 1.e6; // s->us

    m_ibdNum[dEvt->m_det-1]->Fill(dEvt->m_trigTime.AsDouble());
    m_nibd[dEvt->m_det-1]++;

    if(m_saveTree) {
      m_tep = pEvt->energy();
      m_txp = pEvt->m_x;
      m_typ = pEvt->m_y;
      m_tzp = pEvt->m_z;
      m_ten = dEvt->energy();
      m_txn = dEvt->m_x;
      m_tyn = dEvt->m_y;
      m_tzn = dEvt->m_z;
      m_tdt = timeInt;
      m_tdet = dEvt->m_det;
      m_tpfile = pEvt->m_fileNum;
      m_tpentry = pEvt->m_localEntry;
      m_tnfile = dEvt->m_fileNum;
      m_tnentry = dEvt->m_localEntry;
      m_tptrignum = pEvt->m_trigNum;
      m_tntrignum = dEvt->m_trigNum;
      m_tn = dEvt->m_trigTime.GetSec();
      m_tree->Fill();
    }

    LogInfo << "Find an IBD candidate in (file, local entry, trigNum) =  (" 
      << pEvt->m_fileNum << "," << pEvt->m_localEntry << "," << pEvt->m_trigNum << "), "
      << dEvt->m_fileNum << "," << dEvt->m_localEntry << "," << dEvt->m_trigNum << "), "
      << endl;
    //LogInfo << "Find an IBD candidate in file " << pEvt->m_fileNum
    //  << ", local entry " << pEvt->m_localEntry << ", trigNum " << pEvt->m_trigNum
    //  << ", and file " << dEvt->m_fileNum
    //  << ", local entry " << dEvt->m_localEntry << ", trigNum " << dEvt->m_trigNum
    //  << endl;
    LogInfo << "Find an IBD candidate with (det, ep, ed, dt) = ("
      << pEvt->m_det << "," << pEvt->energy() << "," << dEvt->energy()
      << "," << timeInt << ")." << endl;
  } else {
    LogDebug << "Energy cut is not satisfied." << endl;
  }

  return true;
}

bool IBDSelectAlg::fillSingles(const vector<PhyEvent*> &evtGroup) {

  PhyEvent *evt = evtGroup[0];

  //Apply muon veto on delayed signal
  if(evt->m_userTag["MuonVeto"]>0) {
    LogDebug << "Veto delayed signal by muon." << endl;
    return true;
  }

  // energy cut for prompt like singles
  if(evt->energy()>m_eplow && evt->energy()<m_ephigh) {
    m_npsingles[evt->m_det-1]++;
    m_hPSingles[evt->m_det-1]->Fill(evt->energy());
  }

  // energy cut for neutron like singles
  if(evt->energy()>m_edlow && evt->energy()<m_edhigh) {
    m_nnsingles[evt->m_det-1]++;
    m_hNSingles[evt->m_det-1]->Fill(evt->energy());
  }

  return true;
}
