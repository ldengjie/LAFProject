#include "OffWindowAlg.h"
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

DECLARE_ALGORITHM(OffWindowAlg);

OffWindowAlg::OffWindowAlg(const std::string& name)
: AlgBase(name)
{
  setOption("PromptEngLow",m_eplow);  
  setOption("PromptEngHigh",m_ephigh);  
  setOption("DelayedEngLow",m_edlow);  
  setOption("DelayedEngHigh",m_edhigh);  
  setOption("DtLow",m_dtlow);  
  setOption("DtHigh",m_dthigh);  
  setOption("OffWindow",m_offWindow);
  setOption("OffWindowBin",m_offWindowBin);

  for(int det=1;det<=GlobalVar::NumADs;det++) {
    m_adSingleBuf[det-1].clear();    
  }
}

bool OffWindowAlg::initialize()
{
  LogInfo << "initialize()" << endl;
  m_buffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
  m_liveTimeSvc = dynamic_cast<LiveTimeSvc*>(service("LiveTimeSvc"));
  if(!m_liveTimeSvc) {
    LogError << "Can't get LiveTimeSvc." << endl;
    return false;
  }
  m_muonVeto = MuonVeto::instance();

  m_tree = new TTree("offw","offw");
  m_tree->Branch("method",&m_offwMethod,"method/I");
  m_tree->Branch("ioffw",&m_ioffw,"ioffw/I");
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

  for(int det=1;det<=GlobalVar::NumADs;det++) {
    ostringstream sns,snc;
    sns << "OffWinAD" << det << "ns";
    snc << "OffWinAD" << det << "nc";
    m_hNSingles[det-1] = new TH1F(sns.str().c_str(),"Neutron like singles",100,m_edlow,m_edhigh);
    m_hNCorr[det-1] = new TH1F(snc.str().c_str(),"Neutron from correlated events",100,m_edlow,m_edhigh);
    ntupleSvc()->attach("FILE1/hem/Singles/",m_hNSingles[det-1]);
    ntupleSvc()->attach("FILE1/hem/Singles/",m_hNCorr[det-1]);
  }

  return true;
}

bool OffWindowAlg::finalize()
{
  LogInfo << "finalize()" << endl;
  return true;
}

bool OffWindowAlg::execute()
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
    for(unsigned int j=0;j<m_adSingleBuf[det-1].size();j++) {
      LogDebug << j << "," << m_adSingleBuf[det-1][j] << "," 
        << m_adSingleBuf[det-1][j]->m_det << "," << m_adSingleBuf[det-1][j]->m_entry << endl;
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
  if(isVetoedByWp(event)) return true;

  // fill remaining events in a buffer for off-window accidental background caculation
  m_adSingleBuf[event->m_det-1].push_back(event->GrabInstance());
  fillOffWinAcc(event);

  return true;
}

// use off window methed to determine accidental background
bool OffWindowAlg::fillOffWinAcc(PhyEvent* event) {

  // only keep 10ms+200us events in m_adSingleBuf
  vector<PhyEvent*>::iterator it = m_adSingleBuf[event->m_det-1].begin();
  while( it != m_adSingleBuf[event->m_det-1].end() ) {
    PhyEvent* evt = (*it);
    double dt = event->m_trigTime - evt->m_trigTime;
    if(dt > m_offWindow + (m_offWindowBin+1)*m_dthigh) {
      evt->ReleaseInstance();
      it = m_adSingleBuf[event->m_det-1].erase(it);
    } else {
      break;
    }
  }

  // muon veto
  // WP muon: [-2us,600us]
  // AD muon: [0,1ms]
  // AD shower muon: [0,1s]
  MuonVeto *muonVeto = MuonVeto::instance();
  if(muonVeto->veto(event)) {
    return true;
  }

  // only keep neutron like event
  if(event->energy() <= m_edlow || event->energy() >= m_edhigh) {
    return true;
  }

  // require no >0.7MeV event in following 200us window
  DataBuffer<PhyEvent>::Iterator itdb=m_buffer->find(event);
  itdb++;
  while(itdb < m_buffer->end()) {
    PhyEvent* evt = (*itdb);
    double dt = evt->m_trigTime - event->m_trigTime;
    if(dt > m_dthigh) break;
    if(evt->m_good && evt->m_det==event->m_det && !isVetoedByWp(evt)) { 
      return true;
    }
    itdb++;
  }

  // used for correction in methed 1, see doc:762
  m_hNSingles[event->m_det-1]->Fill(event->energy());

  // require no >0.7MeV event in previous 200us window for method 2
  m_offwMethod = 2;
  itdb=m_buffer->find(event);
  itdb--;
  while(itdb >= m_buffer->begin()) {
    PhyEvent* evt = (*itdb);
    double dt = event->m_trigTime - evt->m_trigTime;
    if(dt > m_dthigh) break;
    if(evt->m_good && evt->m_det==event->m_det && !isVetoedByWp(evt)) { 
      m_offwMethod = 1;
      m_hNCorr[event->m_det-1]->Fill(event->energy());
    }
    itdb--;
  }

  it = m_adSingleBuf[event->m_det-1].end();
  it--; 
  it--; // loop backward start from previous event of single neutron

  int evtInOffWindow[m_offWindowBin];
  for(int i=0; i<m_offWindowBin; i++) {
    evtInOffWindow[i]=0;
  }
  //caculate number of events in each off-window
  while( it >= m_adSingleBuf[event->m_det-1].begin() ) {
    double dt = event->m_trigTime - (*it)->m_trigTime;
    for(int i=0; i<m_offWindowBin; i++) {
      if(dt < m_offWindow + (i+1)*m_dthigh && dt > m_offWindow + i*m_dthigh) {
        evtInOffWindow[i]++;
        break;
      }
    }
    it--;
  }

  it = m_adSingleBuf[event->m_det-1].end();
  it--; 
  it--; // loop backward start from previous event of single neutron
  while( it >= m_adSingleBuf[event->m_det-1].begin() ) {
    PhyEvent* evt = (*it);
    double dt = event->m_trigTime - evt->m_trigTime;
    LogDebug << "local entry: " << evt->m_localEntry << ", dt=" << dt << endl;
    for(int i=0; i<m_offWindowBin; i++) {
      if(evtInOffWindow[i] > 1) continue;  //multiplicity cut
      if(dt < m_offWindow + (i+1)*m_dthigh && dt > m_offWindow + m_dtlow + i*m_dthigh) {
        if(evt->energy() >= m_ephigh) continue;
        bool offAcc = false;
        if(it == m_adSingleBuf[event->m_det-1].begin()) {
          LogDebug << "This is the first event in buffer: " << evt->m_localEntry << endl;
          offAcc = true;  // which means evt is the first event in the buffer
        } else {
          vector<PhyEvent*>::iterator it1 = it;
          it1--;  //find previous event
          double dt1 = evt->m_trigTime - (*it1)->m_trigTime;
          if(dt1 > m_dthigh) {
            offAcc = true;  // which means no events in previous 200us window
          }
        }
        if(offAcc) {
          m_ioffw = i;
          m_tep = evt->energy();
          m_txp = evt->m_x;
          m_typ = evt->m_y;
          m_tzp = evt->m_z;
          m_ten = event->energy();
          m_txn = event->m_x;
          m_tyn = event->m_y;
          m_tzn = event->m_z;
          m_tdt = dt*1000.; // s->ms
          m_tdet = event->m_det;
          m_tpfile = evt->m_fileNum;
          m_tpentry = evt->m_localEntry;
          m_tnfile = event->m_fileNum;
          m_tnentry = event->m_localEntry;
          m_tptrignum = evt->m_trigNum;
          m_tntrignum = event->m_trigNum;
          m_tn = event->m_trigTime.GetSec();
          m_tree->Fill();
          if(i==0) {
            LogDebug << "Find an Acc candidate in file " << evt->m_fileNum
              << ", local entry " << evt->m_localEntry //<< ", event " << evt->m_event
              << ", and file " << event->m_fileNum
              << ", local entry " << event->m_localEntry //<< ", event " << event->m_event
              << endl;
            LogDebug << "Find an Acc candidate with (det, ep, ed, dt) = ("
              << evt->m_det << "," << evt->energy() << "," << event->energy()
              << "," << dt << endl;
          }
        }
        continue;
      }
    }
    it--;
  }

  return true;
}

// if event vetoed by [-2us,200us] of WP muon
bool OffWindowAlg::isVetoedByWp(PhyEvent *event) {
  bool veto = false;
  DataBuffer<PhyEvent>::Iterator it=m_buffer->find(event);
  it--;
  while ( it >= m_buffer->begin() ) {
    PhyEvent* evt = (*it);
    double dt = event->m_trigTime - evt->m_trigTime;
    if(dt>2.e-4) break;
    if(evt->isMuon() && (evt->m_det==5 || evt->m_det==6)) {
      if(dt <= 2.e-4) {
        veto = true;
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
        veto = true;
      }
    }
    it++;
  }

  return veto;
}
