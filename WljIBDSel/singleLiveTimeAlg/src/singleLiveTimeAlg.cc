#include "singleLiveTimeAlg.h"
#include "LafKernel/AlgFactory.h"
#include "LafKernel/GlobalVar.h"
#include <iostream>
#include "TMath.h"

using namespace std;

DECLARE_ALGORITHM(singleLiveTimeAlg);

singleLiveTimeAlg::singleLiveTimeAlg(const std::string& name)
: AlgBase(name)
{
  OptionParser::setOption(name, "MuVetoWindow",m_muVetoWin=2.e-4);
  OptionParser::setOption(name, "AdMuVetoWindow",m_adMuVetoWin=1.e-3);
  OptionParser::setOption(name, "ShowerMuVetoWindow",m_showerMuVetoWin=1.0);
  OptionParser::setOption(name, "PreVetoWindow",m_preVetoWin=2.e-6);

  OptionParser::setOption(name, "useRPCVeto", m_useRPCVeto=true);

  for(int det=1;det<=GlobalVar::NumADs;det++) {
    m_deadTime[0][det-1] = 0.;
    m_deadTime[1][det-1] = 0.;
    m_vetoWindowPost[0][det-1] = 0;
    m_vetoWindowPost[1][det-1] = 0;
  }
  m_muonVeto = MuonVeto::instance();
}

bool singleLiveTimeAlg::initialize()
{
  // ================ Define histograms ========================
  // Binning
  const int nTimeBinNum = 6000;
  double timeBins[nTimeBinNum+1];
  double timeBinWidth[nTimeBinNum+1];
  timeBins[0] = 1313280000;
  for(int i=0; i<=nTimeBinNum; i++) {
    timeBinWidth[i] = 3600.;
    if(i>0) timeBins[i] = timeBins[i-1] + timeBinWidth[i];
  }

  daqLiveTime = new TH1D("daqLiveTime", "daqLiveTime", nTimeBinNum, timeBins);
  ntupleSvc()->attach("FILE1/wenlj", daqLiveTime);
  TString histname;
  for(Int_t i=1; i<=GlobalVar::NumADs; i++) {
    histname = "vetoTimeForTrigPlots"; 
    histname += "AD"; histname += i;
    vetoTimeForTrigPlots[i-1] = new TH1D(histname, "vetoTimeForTrigPlots", nTimeBinNum, timeBins);
    ntupleSvc()->attach("FILE1/wenlj", vetoTimeForTrigPlots[i-1]);

    histname = "vetoTimeForIBDSel"; 
    histname += "AD"; histname += i;
    vetoTimeForIBDSel[i-1] = new TH1D(histname, "vetoTimeForIBDSel", nTimeBinNum, timeBins);
    ntupleSvc()->attach("FILE1/wenlj", vetoTimeForIBDSel[i-1]);

    histname = "gapTime";
    histname += "AD"; histname += i;
    gapTime[i-1] = new TH1D(histname, "time gap", nTimeBinNum, timeBins);
    ntupleSvc()->attach("FILE1/wenlj", gapTime[i-1]);
  }

  m_buffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));

  m_tree = ntupleSvc()->bookTree("FILE3/liveTime", "liveTime"); // live time tree
  m_tree->Branch("det", &m_det, "det/I"); 
  m_tree->Branch("timeBin", &m_timeBin, "timeBin/I"); 
  m_tree->Branch("liveWindowSec", &m_liveWindowSec, "liveWindowSec/I"); 
  m_tree->Branch("liveWindowNanoSec", &m_liveWindowNanoSec, "liveWindowNanoSec/I"); 


  return true;
}

bool singleLiveTimeAlg::finalize()
{
  updateDaqLiveTime();
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    double dT;
    dT = (m_endTime - m_vetoWindowPost[1][det-1]);
    cout << "dT: " << dT << endl;
    if(dT>0.) {
      cout << "the last live time window" << endl;
      m_liveWindowSec = (int) dT; // sec part 
      m_liveWindowNanoSec = (int) ((dT - m_liveWindowSec)*1e9); // nanosec part
      m_det = det;
      m_timeBin = daqLiveTime->FindBin(m_endTime.GetSec());
      m_tree->Fill();
    }
  }

  return true;
}

bool singleLiveTimeAlg::execute()
{
  // If bad event, return
  PhyEvent *event = m_buffer->curEvt();
  if(event->m_localEntry== 0) {
    if(event->m_entry==0) m_lastFileNum = event->m_fileNum;
    // update daqLiveTime once a get to a new file
    // the daqLiveTime of the last file will be updated in finalize()
    if(event->m_fileNum > m_lastFileNum)  {
      double timegap = event->m_trigTime - m_endTime;
      //if(timegap>0.5) { 
      if(timegap>0.5) { 
        LogInfo<< "--timeGap----: currentEvt: " 
                << event->m_localEntry << " trigTime: " 
                <<  event->m_trigTime << ", preEvt: " 
                << m_endTime << ",  timegap: " << timegap 
                << endl;
        updateDaqLiveTime();

        for(int det=1;det<=GlobalVar::NumADs;det++) {
          double dT;
          dT = (m_endTime - m_vetoWindowPost[1][det-1]);
          cout << "dT: " << dT << endl;
          if(dT>0.) {
            cout << "the last live time window" << endl;
            m_liveWindowSec = (int) dT; // sec part 
            m_liveWindowNanoSec = (int) ((dT - m_liveWindowSec)*1e9); // nanosec part
            m_det = det;
            m_timeBin = daqLiveTime->FindBin(m_endTime.GetSec());
            m_tree->Fill();
          }
          m_vetoWindowPost[1][det-1] = 0;
          m_vetoWindowPost[0][det-1] = 0;
        }
      }
      else {
        m_endTime = event->m_trigTime;
        updateDaqLiveTime();
      }

    }

    m_beginTime = event->m_trigTime;
    m_endTime = event->m_trigTime;
    m_lastFileNum = event->m_fileNum;

    if(!event->m_good) return true;
  } 
  else {
    double timegap = event->m_trigTime - m_endTime;
    if(timegap>0.5) { 
      LogInfo << "--timeGap----: currentEvt: " 
              << event->m_localEntry << " detector: " 
              << event->m_det << " trigTime: " 
              <<  event->m_trigTime << ", preEvt: " 
              << m_endTime << ",  timegap: " << timegap 
              << endl;
      if(event->m_det<=4) gapTime[event->m_det-1]->Fill(event->m_trigTime.GetSec(), timegap);
      else {
        for(int det=1;det<=GlobalVar::NumADs;det++)
          gapTime[det-1]->Fill(event->m_trigTime.GetSec(), timegap);
      }
    }
    m_endTime = event->m_trigTime;
  }
  if(event->m_entry%100000==0) cout << "------entry=" << event->m_entry << endl;

//  LogDebug << "------entry=" << event->m_entry
//    << ", " << event
//    << ", fileNum=" << event->m_fileNum
//    << ", local entry=" << event->m_localEntry
//    << ", trigtime=" << event->m_trigTime
//    << ", det=" << event->m_det 
//    << ", energy=" << event->m_energy
//    << ", rawEvis=" << event->m_rawEvis
//    << ", energy()=" << event->energy()
//    << ", nPmt=" << event->m_nPmt
//    << ", flasherTag=" << event->m_flasherTag 
//    << ", forceTrigTag=" << event->m_forceTrigTag
//    << ", crossTrigTag=" << event->m_crossTrigTag
//    << ", rpcNoiseTag=" << event->m_rpcNoiseTag
//    << ", adLowEnergyTag=" << event->m_adLowEnergyTag
//    << ", goodevent=" << event->m_good
//    << ", muontag=" << event->m_muonTag
//    << endl;

  // update muon veto window
  if(event->isMuon()) {
//    LogDebug << "This is a muon. Update the veto window!" << endl;
    updateVetoWindow(event);
  }

  return true;
}

const TTimeStamp singleLiveTimeAlg::timeCnv(double time)
{
  int sec = (int)time;
  int nanosec = (int)((time-sec)*1.e9);
  return TTimeStamp(sec,nanosec);
}

void singleLiveTimeAlg::updateVetoWindow(PhyEvent *event)
{
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    if(!event->isMuon()) continue;
//    LogDebug << "det=" << det << "," << event->m_muonTag << std::endl;

    TTimeStamp newVetoWindowPost[2];
    // indx 0:  for trigger rate plots, use settings in MuonVeto package
    // indx 1:  for IBD selection
    newVetoWindowPost[0] = event->m_trigTime;
    newVetoWindowPost[1] = event->m_trigTime;

    if(event->isWpMuon()) {
      newVetoWindowPost[0].Add(timeCnv(m_muonVeto->muVetoWin()));
      newVetoWindowPost[1].Add(timeCnv(m_muVetoWin));
    } else if(event->isRpcMuon() && m_useRPCVeto) { 
      newVetoWindowPost[0].Add(timeCnv(m_muonVeto->muVetoWin()));
      newVetoWindowPost[1].Add(timeCnv(m_muVetoWin));
    } else if(event->isAdMuon() && event->m_det==det) { 
      newVetoWindowPost[0].Add(timeCnv(m_muonVeto->adMuVetoWin()));
      newVetoWindowPost[1].Add(timeCnv(m_adMuVetoWin));
    } else if(event->isShowerMuon() && event->m_det==det) {
      newVetoWindowPost[0].Add(timeCnv(m_muonVeto->showerMuVetoWin()));
      newVetoWindowPost[1].Add(timeCnv(m_showerMuVetoWin));
    } else {
      continue;
    }

    TTimeStamp newVetoWindowPre[2];
    newVetoWindowPre[0] = event->m_trigTime;
    newVetoWindowPre[1] = event->m_trigTime;
    // Apply pre veto window for WP or RPC muon
    if(event->isWP() || event->isRPC()) {
      newVetoWindowPre[0].Add(timeCnv(-m_muonVeto->preVetoWin()));
      newVetoWindowPre[1].Add(timeCnv(-m_preVetoWin));
    }

//    LogDebug << "Current time=" << event->m_trigTime
//      << ", veto window pre (for trigger rate plots) =" << newVetoWindowPre[0]
//      << ", veto window post (for trigger rate plots) =" << newVetoWindowPost[0]
//      << ", veto window pre (for IBD selection)=" << newVetoWindowPre[1]
//      << ", veto window post (for IBD selection) =" << newVetoWindowPost[1] << std::endl;

    // The first step of muon veto
    if(newVetoWindowPost[0] > m_vetoWindowPost[0][det-1]) {
//      LogDebug << "Update muon veto window to " << newVetoWindowPost[0]
//        << " in detector " << event->m_det << endl;
      if(newVetoWindowPre[0] > m_vetoWindowPost[0][det-1]) {
        m_deadTime[0][det-1] += (newVetoWindowPost[0] - newVetoWindowPre[0]);
        vetoTimeForTrigPlots[det-1]->Fill(event->m_trigTime.GetSec(), 
                             newVetoWindowPost[0] - newVetoWindowPre[0]);
//        LogDebug << (newVetoWindowPost[0] - event->m_trigTime) << std::endl;
      } else {
        m_deadTime[0][det-1] += (newVetoWindowPost[0] - m_vetoWindowPost[0][det-1]);
        vetoTimeForTrigPlots[det-1]->Fill(event->m_trigTime.GetSec(), 
                             newVetoWindowPost[0] - m_vetoWindowPost[0][det-1]);
      }
      m_vetoWindowPost[0][det-1] = newVetoWindowPost[0];
    }

    // muon veto for IBD Selection
    if(newVetoWindowPost[1] > m_vetoWindowPost[1][det-1]) {
//      LogDebug << "Update muon veto window to " << newVetoWindowPost[1]
//        << " in detector " << event->m_det << endl;
      if(newVetoWindowPre[1] > m_vetoWindowPost[1][det-1]) {
        m_deadTime[1][det-1] += (newVetoWindowPost[1] - newVetoWindowPre[1]);
        vetoTimeForIBDSel[det-1]->Fill(event->m_trigTime.GetSec(), 
                               newVetoWindowPost[1] - newVetoWindowPre[1]);
//        LogDebug << (newVetoWindowPost[1] - event->m_trigTime) << std::endl;
        
        double dT;
        dT = (newVetoWindowPre[1] - m_vetoWindowPost[1][det-1]);
        if(dT>1e9) dT = newVetoWindowPre[1] - m_beginTime;
        m_timeBin = daqLiveTime->FindBin(event->m_trigTime.GetSec());
        m_liveWindowSec = (int) dT; // sec part 
        m_liveWindowNanoSec = (int) ((dT - m_liveWindowSec)*1e9); // nanosec part
        m_det = det;
        m_tree->Fill();

        if(m_liveWindowSec>2.0) LogInfo << "trigSec, liveWindowSec, liveWindowNano: " << event->m_trigTime.GetSec() << ", " << m_liveWindowSec << ", " << m_liveWindowNanoSec << endl;
        if(m_liveWindowSec<0.) LogInfo << "WARN: liveWindowSec<0! trigSec, liveWindowSec, liveWindowNano: " << event->m_trigTime.GetSec() << ", " << m_liveWindowSec << ", " << m_liveWindowNanoSec << endl;
      } else {
        m_deadTime[1][det-1] += (newVetoWindowPost[1] - m_vetoWindowPost[1][det-1]);
        vetoTimeForIBDSel[det-1]->Fill(event->m_trigTime.GetSec(), 
                               newVetoWindowPost[1] - m_vetoWindowPost[1][det-1]);
      }
      m_vetoWindowPost[1][det-1] = newVetoWindowPost[1];
    }
  }
}

void singleLiveTimeAlg::updateDaqLiveTime() {
  // Daq live time histograming
  double totalTime = m_endTime - m_beginTime;
  int startBin = daqLiveTime->FindBin(m_beginTime.GetSec());
  int endBin = daqLiveTime->FindBin(m_endTime.GetSec());
  LogDebug << "LiveTime update: (begin, end, total): "<< m_beginTime << ",  " << m_endTime << ", " << totalTime << endl;
  if(endBin > startBin) {
    double timeTmp = m_endTime - timeCnv(daqLiveTime->GetBinLowEdge(endBin));
    daqLiveTime->Fill(m_endTime.GetSec(), timeTmp);

    timeTmp = m_beginTime - timeCnv(daqLiveTime->GetBinLowEdge(startBin));
    daqLiveTime->Fill(m_beginTime.GetSec(), daqLiveTime->GetBinWidth(startBin) - timeTmp);

    for(int i=1; i<endBin-startBin; i++) {
      daqLiveTime->Fill(daqLiveTime->GetBinCenter(startBin+i), daqLiveTime->GetBinWidth(startBin+i));
    }
  }
  else {
    daqLiveTime->Fill(m_beginTime.GetSec(), totalTime);
  }
}
