#include "SinglesAlg.h"
#include "MuonVeto/MuonVeto.h"
#include "LafKernel/AlgFactory.h"
#include "LafKernel/GlobalVar.h"
#include "TMath.h"
#include <iostream>

using namespace std;

DECLARE_ALGORITHM(SinglesAlg);

SinglesAlg::SinglesAlg(const std::string& name)
: AlgBase(name)
{
  setOption("SinglesEngLow", m_eSinglesLow);
  setOption("SinglesEngHigh", m_eSinglesHigh);
  setOption("SinglesTime2PreLow", m_t2preLow);
  setOption("SinglesTime2PostLow", m_t2postLow);
  setOption("SingleTime2MuonHigh", m_t2MuonHigh);
  setOption("FindSingle", m_findSingle = true);

  for(int det=1;det<=GlobalVar::NumADs;det++) {
    m_adSingles[det-1].clear();
    m_lastSingle[det-1] = 0;
  }
}

bool SinglesAlg::initialize()
{
  char name[100];
  m_buffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));

  m_muonNeutronBufSvc = dynamic_cast<MuonNeutronBufSvc*>(service("MuonNeutronBufSvc"));

  m_liveTimeSvc = dynamic_cast<LiveTimeSvc*>(service("LiveTimeSvc"));
  TTimeStamp start = m_liveTimeSvc->startTime();
  TTimeStamp end   = m_liveTimeSvc->endTime();
  int        nBins = m_liveTimeSvc->nBins();
  TString myDir = "FILE1/liuyb";
  if(m_findSingle){
    // Singles tree
    m_tSingles = ntupleSvc()->bookTree((myDir + TString("/Singles")).Data(), "SinglesTree");
    m_tSingles->Branch("Det", &m_det, "Det/I");
    m_tSingles->Branch("Time2Pre", &m_t2pre, "Time2Pre/F");
    m_tSingles->Branch("Time2Post", &m_t2post, "Time2Post/F");
    m_tSingles->Branch("PreEnergy", &m_ePre, "PreEnergy/F");
    m_tSingles->Branch("PostEnergy", &m_ePost, "PostEnergy/F");
    m_tSingles->Branch("TrigSec", &m_trigSec, "TrigSec/I");
    m_tSingles->Branch("TrigNanoSec", &m_trigNanoSec, "TrigNanoSec/I");
    m_tSingles->Branch("Enrec", &m_enrec, "Enrec/F");
    m_tSingles->Branch("X", &m_x, "X/F");
    m_tSingles->Branch("Y", &m_y, "Y/F");
    m_tSingles->Branch("Z", &m_z, "Z/F");
    m_tSingles->Branch("XPre", &m_xPre, "XPre/F");
    m_tSingles->Branch("YPre", &m_yPre, "YPre/F");
    m_tSingles->Branch("ZPre", &m_zPre, "ZPre/F");
    m_tSingles->Branch("XPost", &m_xPost, "XPost/F");
    m_tSingles->Branch("YPost", &m_yPost, "YPost/F");
    m_tSingles->Branch("ZPost", &m_zPost, "ZPost/F");
    m_tSingles->Branch("MuonMultiplicity", &m_muonMulti, "MuonMultiplicity/I");
    m_tSingles->Branch("MuonEnergy", m_eMuon, "MuonEnergy[MuonMultiplicity]/F");
    m_tSingles->Branch("SingleTime2Muon", m_t2Muon, "SingleTime2Muon[MuonMultiplicity]/F");
    m_tSingles->Branch("NeuMultiplicityHGd", m_nPostNeuHGd, "NeuMultiplicityHGd[MuonMultiplicity]/I");
    m_tSingles->Branch("Quadrant", &m_quadrant, "Quadrant/F");
    m_tSingles->Branch("MaxQ", &m_maxQ, "MaxQ/F");
  }

  for(int i = 0; i < GlobalVar::NumADs; i++){
    sprintf(name, "MuonSpec_AD%d", i + 1);
    m_hMuonSpec[i] = new TH1D(name, name, 5000, 0, 5); // GeV
    ntupleSvc()->attach(myDir.Data(), m_hMuonSpec[i]);

    sprintf(name, "MuonSpecAfterReduction_AD%d", i + 1);
    m_hMuonSpecAftRed[i] = new TH1D(name, name, 5000, 0, 5); // GeV
    ntupleSvc()->attach(myDir.Data(), m_hMuonSpecAftRed[i]);

    sprintf(name, "NeutronMultiplicity_AD%d", i + 1);
    m_hNeutronMultiplicity[i] = new TH1D(name, name, 100, 0, 100);
    ntupleSvc()->attach(myDir.Data(), m_hNeutronMultiplicity[i]);

    sprintf(name, "MuonEnergy_vs_NeutronMultiplicity_AD%d", i + 1);
    m_hMuonEnergyVsNeuMulti[i] = new TH2D(name, name, 5000, 0, 5, 100, 0, 100);
    ntupleSvc()->attach(myDir.Data(), m_hMuonEnergyVsNeuMulti[i]);

    sprintf(name, "SinglesSpec_AD%d", i + 1);
    m_hSinglesSpec[i] = new TH1D(name, name, 80, 0, 20); // MeV
    ntupleSvc()->attach(myDir.Data(), m_hSinglesSpec[i]);

    sprintf(name, "Time2LastSingle_AD%d", i + 1);
    m_hTime2LastSingle[i] = new TH1D(name, name, 50*1000, 0, 50);
    ntupleSvc()->attach(myDir.Data(), m_hTime2LastSingle[i]);

    sprintf(name, "SinglesRate_AD%d", i + 1);
    m_hSinglesRate[i] = new TH1D(name, name, nBins, start.AsDouble(), end.AsDouble());
    ntupleSvc()->attach(myDir.Data(), m_hSinglesRate[i]);

    sprintf(name, "SinglesYvsX_AD%d", i + 1);
    m_hSinglesYvsX[i] = new TH2D(name, name, 600, -3, 3, 600, -3, 3); //m:m
    ntupleSvc()->attach(myDir.Data(), m_hSinglesYvsX[i]);

    sprintf(name, "SinglesZvsR2_AD%d", i + 1);
    m_hSinglesZvsR2[i] = new TH2D(name, name, 800, 0, 8, 500, -2.5, 2.5); // m^2:m
    ntupleSvc()->attach(myDir.Data(), m_hSinglesZvsR2[i]);

    sprintf(name, "TriggerSpec_afterMuonVeto_AD%d", i + 1);
    m_hAdTrigSpec_afterMuonVeto[i] = new TH1D(name, name, 200, 0, 20);
    ntupleSvc()->attach(myDir.Data(), m_hAdTrigSpec_afterMuonVeto[i]);

    sprintf(name, "TriggerSpec_noMuonVeto_AD%d", i + 1);
    m_hAdTrigSpec_noMuonVeto[i] = new TH1D(name, name, 200, 0, 20);
    ntupleSvc()->attach(myDir.Data(), m_hAdTrigSpec_noMuonVeto[i]);

  }

  return true;
}

bool SinglesAlg::finalize()
{
  return true;
}

bool SinglesAlg::execute()
{
  // Get an event from the main buffer
  PhyEvent *event = m_buffer->curEvt();
  LogDebug << "------entry=" << event->m_entry
    << ", fileNum=" << event->m_fileNum
    << ", local entry=" << event->m_localEntry
    << ", trigtime=" << event->m_trigTime
    << ", det=" << event->m_det 
    << ", energy=" << event->energy()
    << ", flasherTag=" << event->m_flasherTag
    << ", forceTrigTag=" << event->m_forceTrigTag
    << ", crossTrigTag=" << event->m_crossTrigTag
    << ", rpcNoiseTag=" << event->m_rpcNoiseTag
    << ", goodevent=" << event->m_good
    << ", muontag=" << event->m_muonTag
    << endl;

  if(!event->m_good){
    LogDebug << "Skip bad event" << endl;
    return true;
  }
 
  if(!event->isAD()){
    LogDebug << "Skip non-AD event" << endl;
    return true;
  }

  m_muonNeutronBufSvc->updateBuf(event);

  // For now, 'execute' returns when it comes to a muon event or an event in the muon veto window. In priciple, this conflicts with the isolating cut, which requires 'no triggers' 200us before nor after the current single trigger

  if(event->isMuon()){// Here muon means each >20MeV AD trigger
    LogDebug << "Skip muon event, entry=" << event->m_entry << endl;
    EvtDEvtPD* muonNeutronBuf = m_muonNeutronBufSvc->getMuonNeutronBuf(event->m_det);// not det
    int size = muonNeutronBuf->size();
    // when finding a new muon, get the info of last muon and fill it into hists
    if(size > 1){
      EvtDEvtPDIter it = muonNeutronBuf->begin();
      PhyEvent* muon = muonNeutronBuf->at(size-2).first;
      EvtD neuD = muonNeutronBuf->at(size-2).second;
      m_hMuonSpec[event->m_det - 1]->Fill(muon->energy()/1000.);
      if(neuD.size() > 0){
        m_hMuonSpecAftRed[event->m_det-1]->Fill(muon->energy()/1000.);
      }
    }
    return true;
  }

  m_hAdTrigSpec_noMuonVeto[event->m_det - 1]->Fill(event->energy());
  MuonVeto *muonVeto = MuonVeto::instance();
  if(muonVeto->veto(event)){
    LogDebug << "Skip AD event in muon veto window" << endl;
    return true;
  }
  m_hAdTrigSpec_afterMuonVeto[event->m_det - 1]->Fill(event->energy());

  if(m_findSingle){
    int size = m_adSingles[event->m_det-1].size();
    if(size == 0){
      LogDebug << "Buffer for det=" << event->m_det << " is empty." << endl;
      m_adSingles[event->m_det-1].push_back(event->GrabInstance()); // std::deque<PhyEvent*> m_adSingles[4];
    } 
    else if(size == 1){
      LogDebug << "Buffer for det=" << event->m_det << " has only one element." << endl;
      m_adSingles[event->m_det-1].push_back(event->GrabInstance()); // std::deque<PhyEvent*> m_adSingles[4];
    }
    else if(size == 2){
      LogDebug << "Buffer for det=" << event->m_det << " has two elements." << endl;
      m_adSingles[event->m_det-1].push_back(event->GrabInstance()); // std::deque<PhyEvent*> m_adSingles[4];
      // Now 3 events are archived in m_adSingles, check for single event
      int numToBeDelete = fillSingles(m_adSingles[event->m_det-1]);
      for(int i = 0; i < numToBeDelete; i++){
        // Delete the first element int m_adSingles
        m_adSingles[event->m_det-1][0]->ReleaseInstance();
        m_adSingles[event->m_det-1].pop_front();
      }
      LogDebug << "Now the buffer size is " << m_adSingles[event->m_det-1].size() << endl;

    }
    else {
      LogDebug << "Buffer for det=" << event->m_det << " has " 
        << size << " elements,please check." << endl;
    }
  }
    
  return true;
}

int SinglesAlg::fillSingles(const Deque &evtGroup)
{
  LogDebug << "Multiplicity of event group: " << evtGroup.size() << endl;

  PhyEvent *preEvt = evtGroup[0]; // 'pre' is relative to the second element in evtGroup.
  PhyEvent *singleEvt = evtGroup[1]; // the second element in evtGroup can be a potential single event.
  PhyEvent *postEvt = evtGroup[2]; // 'post' is relative to the second element in evtGroup.
  double t2pre = singleEvt->m_trigTime - preEvt->m_trigTime;
  double t2post = postEvt->m_trigTime - singleEvt->m_trigTime;
  double t2first = postEvt->m_trigTime - preEvt->m_trigTime;
  if(t2first < 0 || t2pre < 0 || t2post < 0){
    LogError << "Wrong time order"
      << ", t2first = " << t2first
      << ", t2pre = " << t2pre
      << ", t2post = " << t2post
      << endl ;
  }
  if(t2pre > m_t2preLow && t2post > m_t2postLow){
    const int det = singleEvt->m_det-1;
    m_hSinglesSpec[det]->Fill(singleEvt->energy());

    if(singleEvt->energy() > m_eSinglesLow && singleEvt->energy() < m_eSinglesHigh){
      m_t2pre = t2pre;
      m_t2post = t2post;
      m_ePre = preEvt->energy();
      m_ePost = postEvt->energy();
      m_det = singleEvt->m_det;
      m_enrec = singleEvt->energy();
      m_trigSec = singleEvt->m_trigTime.GetSec();
      m_trigNanoSec = singleEvt->m_trigTime.GetNanoSec();

      m_x = singleEvt->m_x/1000.;
      m_y = singleEvt->m_y/1000.;
      m_z = singleEvt->m_z/1000.;
      m_xPre = preEvt->m_x/1000.;
      m_yPre = preEvt->m_y/1000.;
      m_zPre = preEvt->m_z/1000.;
      m_xPost = postEvt->m_x/1000.;
      m_yPost = postEvt->m_y/1000.;
      m_zPost = postEvt->m_z/1000.;

      //Use MuonNeutronBufSvc to get previous muons and neutrons info
      EvtDEvtPD* muonNeutronBuf = m_muonNeutronBufSvc->getMuonNeutronBuf(singleEvt->m_det);// not det
      for(int i = 0; i < ArrayMaxLen; i++){
        m_t2Muon[i] = -999;
        m_eMuon[i] = 0.;
        m_nPostNeuHGd[i] = 0;
      }
      EvtDEvtPDIter it = muonNeutronBuf->begin();
      m_muonMulti = 0;
      for(; it < muonNeutronBuf->end(); it++){
        PhyEvent* muon = (*it).first;
        EvtD neuD = (*it).second;
        double dt = singleEvt->m_trigTime - muon->m_trigTime;
        if(dt < 0) break;// Because a muon trigger can happen between the single and its following event which is beyond 200us time window.
        if(dt < m_t2MuonHigh){
          m_t2Muon[m_muonMulti] = singleEvt->m_trigTime - muon->m_trigTime;
          m_eMuon[m_muonMulti] = muon->energy();
          m_nPostNeuHGd[m_muonMulti] = neuD.size();
          m_muonMulti++;
        }
      }
      if(m_muonMulti > 0){
        m_quadrant = singleEvt->m_quadrant;
        m_maxQ = singleEvt->m_maxQ;
        m_tSingles->Fill();
        LogDebug << "Find a single event, det=" << singleEvt->m_det
          << ", entry=" << singleEvt->m_entry
          << ", preEntry=" << preEvt->m_entry
          << ", postEntry=" << postEvt->m_entry
          << ", t2pre=" << t2pre
          << ", t2post=" << t2post
          << ", energy=" << singleEvt->energy()
          << endl ;

        m_hSinglesRate[det]->Fill(singleEvt->m_trigTime.AsDouble());
        m_hSinglesYvsX[det]->Fill(m_x, m_y);
        double r2 = m_y*m_y + m_x*m_x;
        m_hSinglesZvsR2[det]->Fill(r2, m_z);

        if(m_lastSingle[det]){
          m_hTime2LastSingle[det]->Fill(singleEvt->m_trigTime - m_lastSingle[det]->m_trigTime);
          m_lastSingle[det]->ReleaseInstance();
        }
        m_lastSingle[det] = singleEvt->GrabInstance();
      }
    }
  }

  // whenever calling fillSingles, at least the first element should be deleted from the buffer
  int numToBeDelete = 1;
  if(t2pre < m_t2postLow){ // if the second elment is too close to the first one, it is not single event too.
    numToBeDelete++;
  }
  if(t2post < m_t2postLow){ // if the last element is too close to the second one, it is not a single event.
    numToBeDelete++;
  }

  return numToBeDelete;
}
