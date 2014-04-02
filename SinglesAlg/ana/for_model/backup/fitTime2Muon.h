#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>
//#include <iterator>
#include <TPad.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1F.h>
#include <TTree.h>
#include <TString.h>
#include <RooDataHist.h>
#include <RooRealVar.h>
#include <RooFormulaVar.h>
#include <RooExponential.h>
#include <RooAddPdf.h>
#include <RooPlot.h>

using namespace std;
using namespace RooFit;

class FitTime2Muon {
  public:
    FitTime2Muon();
    ~FitTime2Muon();
    void SetMuonSliceBoundaries(double* boundaries, int size);
    void SetMuonRates(TString path);
    void SetFitBoundaries(double tmin, double tmax);
    void SetEnergyBoudaries(double emin, double emax);
    void SetSite(TString site);
    void SetHistType(TString type);
    void AddExponentialElement(TString element, double tau);
    void CalculateTimeWindowEff();
    void CalculateEnergyWindowEff();
    void SetIsolationEff();
    void SetInputFile(TString input);
    void SetOutputDirs();

    void InitObservable();
    void InitRooFitCompents();
    double GetMuonSliceRate(int iad, int islice);
    void TakeFittingAction(int adNumFit, int sliceFit);

    void DoFitting(TVirtualPad*, RooAbsPdf*, TH1F*, const int, double rate=0);
    RooAbsPdf* BuildPdfExps  ();
    RooAbsPdf* BuildPdfExpBkg();
  private:
    int m_nSlices;
    double m_tmin;
    double m_tmax;
    double m_emin;
    double m_emax;
    TString m_unit;
    bool m_paramOn; // show param box in the canvas
    TString m_site;
    int     m_adNum;
    int     m_siteId;
    TString m_histType;
    int     m_histTypeId;
    TString m_dirNameForReduction;
    TString m_input;
    TString m_output;
    vector<double> m_muonSlices;
    vector<string> m_muonSlicesString;
    int             m_nElem;
    vector<TString> m_elemTexName;
    vector<TString> m_elemName;
    vector<TString> m_elemNum;
    vector<double>  m_elemTau;
    vector<double>  m_timeWindowEff; // time window efficiencies for each element 
                                     // in the particular time fitting range (tmin, tmax)
    vector<double>  m_energyWindowEff;
    vector<double>  m_muonRates_noRed;
    vector<double>  m_muonRates_red;

    RooRealVar* m_x;
    vector<RooRealVar*>     m_elemTau_R;
    vector<RooFormulaVar*>  m_elemSuperScript_R;
    vector<RooFormulaVar*>  m_combinedSuperScript_R; // element + muon
    vector<RooExponential*> m_elemExp_R;
    vector<RooExponential*> m_combinedExp_R; // element + muon
    vector<RooRealVar*>     m_elemNum_R;
    RooRealVar*             m_muonRate_R;
    RooFormulaVar*          m_muonSuperScript_R;
    RooExponential*         m_muonExp_R;
    RooRealVar*             m_muonNum_R;

    double m_isolationEff[3][4];

    TFile*  m_in_F;
    TFile*  m_out_F;
    TDirectory* m_ads_D[4];
    TDirectory* m_histType_D[4][2];
    TDirectory* m_t2AllMuons_D[4][2];
    TDirectory* m_t2LastMuon_D[4][2];

    TH1F* m_fitRes_AllMuons_H[4][10];
    TH1F* m_fitRes_LastMuon_H[4][10];
    TH1F* m_correctedRes_AllMuons_H[4][10];
    TH1F* m_correctedRes_LastMuon_H[4][10];

    vector<int> m_color;
    vector<int> m_markerstyle;
};
FitTime2Muon::FitTime2Muon(){
  cout << "initializing FitTime2Muon ..." << endl;
  m_tmin = 1.;
  m_tmax = 500.;
  m_unit = "ms";
  m_paramOn = 1;
  m_site = "EH1";
  m_histType = "NoRed";
  m_color.clear();
  m_color.push_back(kBlue);
  m_color.push_back(kRed);
  m_color.push_back(kBlack);
  m_markerstyle.clear();
  m_markerstyle.push_back(20);
  m_markerstyle.push_back(24);
  m_markerstyle.push_back(21);
  m_muonSlices.clear();;
  m_muonSlicesString.clear();;
  m_nElem = 0;
  m_elemTexName.clear();;
  m_elemName.clear();;
  m_elemNum.clear();;
  m_elemTau.clear();;
  m_timeWindowEff.clear();; 
  m_energyWindowEff.clear();;
  m_muonRates_noRed.clear();;
  m_muonRates_red.clear();;
  //m_elemTau_R.clear();
  //m_elemSuperScript_R.clear();
  //m_elemExp_R.clear();
  //m_elemNum_R.clear();
}
FitTime2Muon::~FitTime2Muon(){
  delete m_x; m_x = 0;
  delete m_out_F; m_out_F = 0;
  for(int ielem=0; ielem<m_nElem; ielem++){
    delete m_elemTau_R[ielem];
    delete m_elemSuperScript_R[ielem];
    delete m_combinedSuperScript_R[ielem];
    delete m_elemExp_R[ielem];
    delete m_combinedExp_R[ielem];
    delete m_elemNum_R[ielem];
  }
  m_elemTau_R.clear();
  m_elemSuperScript_R.clear();
  m_combinedSuperScript_R.clear();
  m_elemExp_R.clear();
  m_combinedExp_R.clear();
  m_elemNum_R.clear();
  delete m_muonRate_R;
  delete m_muonSuperScript_R;
  delete m_muonExp_R;
  delete m_muonNum_R;
}
void FitTime2Muon::SetMuonSliceBoundaries(double* boundaries, int size){
  for(int i=0; i<size; i++){
    m_muonSlices.push_back(boundaries[i]);
    if(i<size-1){
      stringstream ss;
      ss << "Muon" << int(boundaries[i]) << "to" << int(boundaries[i+1]);
      m_muonSlicesString.push_back(ss.str());
    }
  }
  m_nSlices = size - 1;
}
void FitTime2Muon::SetMuonRates(TString path){
  ifstream infile(path.Data());
  string line;
  bool flag = 0; // odd lines for no reduction; even lines for after reduction
  while(getline(infile, line)){
    // ignore lines with livetime 
    if(line.find("Reduction") == std::string::npos) continue;

    std::istringstream iss;
    iss.str(line);
    iss >> line;
    for(int i=0; i<m_nSlices; i++){
      double rate;
      iss >> rate;
      if(flag == 0) m_muonRates_noRed.push_back(rate);
      else m_muonRates_red.push_back(rate);
      //cout << rate << endl ;
    }
    flag = !flag;
  }
}
void FitTime2Muon::SetFitBoundaries(double tmin, double tmax){
  m_tmin = tmin;
  m_tmax = tmax;
}
void FitTime2Muon::SetEnergyBoudaries(double emin, double emax){
  m_emin = emin;
  m_emax = emax;
}
void FitTime2Muon::SetSite(TString site){
  m_site = site;
  m_siteId = TString(site(2,3)).Atoi() - 1;// 0, 1, 2
  if(m_site == "EH1"){
    m_adNum = 2;
  } else if(m_site == "EH2"){
    m_adNum = 2;
  } else if(m_site == "EH3"){
    m_adNum = 4;
  }
}
void FitTime2Muon::SetHistType(TString type){
  m_histType = type;
  if(m_histType == "NoRed"){
    m_histTypeId = 0;
    m_dirNameForReduction = "noReduction";
    m_output = Form("FitRes_%sNoRed.root", m_site.Data());
  } else if(m_histType == "Red"){
    m_histTypeId = 1;
    m_dirNameForReduction = "afterReduction";
    m_output = Form("FitRes_%sRed.root", m_site.Data());
  } else {
    cerr << "Unknown histType=" << m_histType << endl;
    exit(-1);
  }
}
void FitTime2Muon::AddExponentialElement(TString element, double tau){
  cout << "FitTime2Muon AddExponentialElement: " << element << endl;
  int size = element.Length();
  int index = 0;
  for(int i=0; i<size; i++){
    // if(!(TString(element[i]).IsAlpha())){
    // if using the line above, error shows up...
    TString ch = element[i];
    if(!(ch.IsAlpha())){
      index = i;
      break;
    }
  }
  
  stringstream ss;
  ss << "N_{^{" << element(index, size-index) << "}" << element(0,index) << "}";
  string elemNum = ss.str();
  m_elemNum.push_back(elemNum);
  string elemName = elemNum.substr(3, elemNum.size()-4);
  m_elemTexName.push_back(elemName);
  m_elemName.push_back(element);
  m_elemTau.push_back(tau);
  m_nElem++;
}
void FitTime2Muon::CalculateTimeWindowEff(){
  TF1* tmpFunc;
  for(int i=0; i<m_nElem; i++){
    tmpFunc = new TF1("tmpFunc", "1./[0]*TMath::Exp(-x/[0])", 0, 100*m_elemTau[i]);
    tmpFunc->SetParameter(0, m_elemTau[i]);
    double eff = tmpFunc->Integral(m_tmin, m_tmax)/tmpFunc->Integral(0, 100*m_elemTau[i]);
    m_timeWindowEff.push_back(eff);
    delete tmpFunc;
  }
}
void FitTime2Muon::CalculateEnergyWindowEff(){
  TFile* tmpF;
  TString path;
  TString directory = "/publicfs/dyb/data/userdata/liuyb/Isotopes/IsotopeSpectra/Chao/beta/spectra/";
  for(int i=0; i<m_nElem; i++){
    if(m_elemTexName[i] == "^{12}B")
      path = directory + "beta/B12tree.root";
    else if(m_elemTexName[i] == "^{9}C")
      path = directory + "EC/C9tree.root";
    else if(m_elemTexName[i] == "^{8}Li")
      path = directory + "beta/Li8tree.root";
    else {
      cerr << "=== CalculateEnergyWindowEff: No TTree file found for " 
        << m_elemTexName[i] << endl;
      exit(-1);
    }
    tmpF = new TFile(path, "read");
    TTree* t = (TTree*)tmpF->Get("PredictedT");
    stringstream ss;
    ss << "energy>" << m_emin << "&&energy<" << m_emax;
    long int selectedEntries = t->GetEntries(ss.str().c_str());
    long int totalEntries = t->GetEntries();
    double eff = selectedEntries*1./totalEntries;
    m_energyWindowEff.push_back(eff);
    delete tmpF;
  }

}
void FitTime2Muon::SetIsolationEff(){

  m_isolationEff[0][0] = 0.976131;
  m_isolationEff[0][1] = 0.976476;
  m_isolationEff[1][0] = 0.977551;
  m_isolationEff[1][1] = 0.977165;
  m_isolationEff[2][0] = 0.977825;
  m_isolationEff[2][1] = 0.977636;
  m_isolationEff[2][2] = 0.977224;
  m_isolationEff[2][3] = 0.977361;
}
void FitTime2Muon::SetInputFile(TString input){
  m_input = input;
}
void FitTime2Muon::SetOutputDirs(){
  m_out_F = new TFile(m_output, "recreate");
  for(int iad=0; iad<m_adNum; iad++){
    TString dirName = Form("%s-AD%d", m_site.Data(), iad+1);
    m_ads_D[iad] = (TDirectory*)m_out_F->mkdir(dirName);
    m_histType_D[iad][m_histTypeId] = (TDirectory*)m_ads_D[iad]->mkdir(m_dirNameForReduction.Data());
    m_t2AllMuons_D[iad][m_histTypeId] = (TDirectory*)m_histType_D[iad][m_histTypeId]->mkdir("t2AllMuons");
    m_t2LastMuon_D[iad][m_histTypeId] = (TDirectory*)m_histType_D[iad][m_histTypeId]->mkdir("t2LastMuon");
  }
}
void FitTime2Muon::InitObservable(){
  m_x = new RooRealVar("Time2Muon", "Time to previous muon", m_tmin, m_tmax, m_unit.Data());
}
double FitTime2Muon::GetMuonSliceRate(int iad, int islice){
  int index;
  if(m_siteId == 0 || m_siteId == 1){
    index = m_siteId*m_adNum*m_nSlices + iad*m_nSlices + islice;
  } else {
    index = m_siteId*(m_adNum-2)*m_nSlices + iad*m_nSlices + islice;
  }
  double rate;
  if(m_histType == "NoRed"){
    rate = m_muonRates_noRed[index];
  } else {
    rate = m_muonRates_red[index];
  }
  return rate;
}
void FitTime2Muon::TakeFittingAction(int adNumFit, int sliceFit){
  int loopAD_start = 0;
  int loopAD_end = m_adNum;
  if(adNumFit <= 0){ // loop for all ADs
    loopAD_start = 0;
    loopAD_end = m_adNum;
  } else if(adNumFit <= m_adNum){ // only for the specific AD
    loopAD_start = adNumFit - 1;
    loopAD_end = adNumFit;
  } else {
    cerr << "====== TakeFittingAction: Wrong adNumFit=" << adNumFit << endl;
    exit(-1);
  }

  int loopSlice_start = 0;
  int loopSlice_end = m_nSlices;
  if(sliceFit <= -1){
    loopSlice_start = 0;
    loopSlice_end = m_nSlices;
  } else if(sliceFit <= m_nSlices){
    loopSlice_start = sliceFit;
    loopSlice_end = sliceFit + 1;
  } else {
    cerr << "====== TakeFittingAction: Wrong sliceFit=" << sliceFit << endl;
    exit(-1);
  }
  const int arrSize = m_nSlices+1;
  double muonSlices[arrSize];
  for(int i=0; i<arrSize; i++) { muonSlices[i] = m_muonSlices[i]; }

  TFile* m_in_F = new TFile(m_input, "read");
  for(int iad=loopAD_start; iad<loopAD_end; iad++){

    TString title;
    for(int ielem=0; ielem<m_nElem; ielem++){
      title = Form("FitRes_t2AllMuons_%s", m_elemName[ielem].Data());
      m_fitRes_AllMuons_H[iad][ielem] = new TH1F(title, "Number of fitted events by time-to-all-muons", arrSize-1, muonSlices);
      title = Form("CorrectedRes_t2AllMuons_%s", m_elemName[ielem].Data());
      m_correctedRes_AllMuons_H[iad][ielem] = new TH1F(title, "Number of events by time-to-all-muons", arrSize-1, muonSlices);
      title = Form("FitRes_t2LastMuon_%s", m_elemName[ielem].Data());
      m_fitRes_LastMuon_H[iad][ielem] = new TH1F(title, "Number of fitted events by time-to-last-muon", m_nSlices, muonSlices);
      title = Form("CorrectedRes_t2LastMuon_%s", m_elemName[ielem].Data());
      m_correctedRes_LastMuon_H[iad][ielem] = new TH1F(title, "Number of events by time-to-last-muon", m_nSlices, muonSlices);
    }

    for(int islice=loopSlice_start; islice<loopSlice_end; islice++){
      int eMuonLow  = int(muonSlices[islice]);
      int eMuonHigh = int(muonSlices[islice + 1]);
      title = Form("%s/%s/Time2LastMuon_%s_ad%d_Muon%dto%d", m_site.Data(), m_muonSlicesString[islice].c_str(), m_dirNameForReduction.Data(), iad+1, eMuonLow, eMuonHigh);
      TH1F* h_t2last = (TH1F*)m_in_F->Get(title);

      title = Form("%s/%s/Time2AllMuon_%s_ad%d_Muon%dto%d", m_site.Data(), m_muonSlicesString[islice].c_str(), m_dirNameForReduction.Data(), iad+1, eMuonLow, eMuonHigh);
      TH1F* h_t2all = (TH1F*)m_in_F->Get(title);

      RooAbsPdf* model;
      TCanvas* cFit;
      RooArgSet* params;
      RooRealVar* centerVal;
      // fit time-to-all-muons
      //cout << "====== BuildPdfExpBkg ..." << endl;
      model = BuildPdfExpBkg();
      int iColor = 0;
      cFit = new TCanvas("cFit", "cFit", 1200, 800);
      DoFitting(cFit, model, h_t2all, iColor);
      // Write objects to ROOT file
      m_t2AllMuons_D[iad][m_histTypeId]->cd();
      cFit->Write("Fit_" + TString(m_muonSlicesString[islice]) + "MeV");
      // Retrieve parameter values after fitting
      params = model->getVariables();
      for(int ielem=0; ielem<m_nElem; ielem++){
        centerVal = (RooRealVar*)params->find(m_elemNum[ielem]);
        m_fitRes_AllMuons_H[iad][ielem]->SetBinContent(islice+1, centerVal->getVal());
        m_fitRes_AllMuons_H[iad][ielem]->SetBinError(islice+1, centerVal->getError());
        double factor = 1./m_timeWindowEff[ielem]/m_energyWindowEff[ielem]/m_isolationEff[m_siteId][iad];
        m_correctedRes_AllMuons_H[iad][ielem]->SetBinContent(islice+1, centerVal->getVal()*factor);
        m_correctedRes_AllMuons_H[iad][ielem]->SetBinError(islice+1, centerVal->getError()*factor);
      }
      //RooRealVar* centerVal = (RooRealVar*)params->find(m_elemNum[0]);
      //m_fitRes_AllMuons_H[iad]->SetBinContent(islice+1, centerVal->getVal());
      //m_fitRes_AllMuons_H[iad]->SetBinError(islice+1, centerVal->getError());
      //m_correctedRes_AllMuons_H[iad]->SetBinContent(islice+1, 
      //    centerVal->getVal()/m_timeWindowEff[0]/m_energyWindowEff[0]/m_isolationEff[m_siteId][iad]);
      //m_correctedRes_AllMuons_H[iad]->SetBinError(islice+1, 
      //    centerVal->getError()/m_timeWindowEff[0]/m_energyWindowEff[0]/m_isolationEff[m_siteId][iad]);
      delete cFit; cFit = 0;
      delete model; model = 0;

      // fit time-to-last-muon
      //cout << "====== BuildPdfExps ..." << endl;
      model = BuildPdfExps();
      cFit = new TCanvas("cFit", "cFit", 1200, 800);
      double rate = GetMuonSliceRate(iad, islice);
      DoFitting(cFit, model, h_t2last, iColor, rate);
      // Write objects to ROOT file
      m_t2LastMuon_D[iad][m_histTypeId]->cd();
      cFit->Write("Fit_" + TString(m_muonSlicesString[islice]) + "MeV");
      // Retrieve parameter values after fitting
      params = model->getVariables();
      for(int ielem=0; ielem<m_nElem; ielem++){
        centerVal = (RooRealVar*)params->find(m_elemNum[ielem]);
        m_fitRes_LastMuon_H[iad][ielem]->SetBinContent(islice+1, centerVal->getVal());
        m_fitRes_LastMuon_H[iad][ielem]->SetBinError(islice+1, centerVal->getError());
        double factor = 1./m_timeWindowEff[ielem]/m_energyWindowEff[ielem]/m_isolationEff[m_siteId][iad];
        m_correctedRes_LastMuon_H[iad][ielem]->SetBinContent(islice+1, centerVal->getVal()*factor);
        m_correctedRes_LastMuon_H[iad][ielem]->SetBinError(islice+1, centerVal->getError()*factor);
      }
      //params = model->getVariables();
      //centerVal = (RooRealVar*)params->find(m_elemNum[0]);
      //m_fitRes_LastMuon_H[iad]->SetBinContent(islice+1, centerVal->getVal());
      //m_fitRes_LastMuon_H[iad]->SetBinError(islice+1, centerVal->getError());
      //m_correctedRes_LastMuon_H[iad]->SetBinContent(islice+1, 
      //    centerVal->getVal()/m_timeWindowEff[0]/m_energyWindowEff[0]/m_isolationEff[m_siteId][iad]);
      //m_correctedRes_LastMuon_H[iad]->SetBinError(islice+1, 
      //    centerVal->getError()/m_timeWindowEff[0]/m_energyWindowEff[0]/m_isolationEff[m_siteId][iad]);
      delete cFit; cFit = 0;
      delete model; model = 0;
    }
    m_t2LastMuon_D[iad][m_histTypeId]->cd();
    for(int ielem=0; ielem<m_nElem; ielem++){
      m_fitRes_LastMuon_H[iad][ielem]->Write();
      m_correctedRes_LastMuon_H[iad][ielem]->Write();
      delete m_fitRes_LastMuon_H[iad][ielem];
      delete m_correctedRes_LastMuon_H[iad][ielem];
    }
    m_t2AllMuons_D[iad][m_histTypeId]->cd();
    for(int ielem=0; ielem<m_nElem; ielem++){
      m_fitRes_AllMuons_H[iad][ielem]->Write();
      m_correctedRes_AllMuons_H[iad][ielem]->Write();
      delete m_fitRes_AllMuons_H[iad][ielem];
      delete m_correctedRes_AllMuons_H[iad][ielem];
    }
  }
  delete m_in_F; m_in_F = 0;
}
void FitTime2Muon::InitRooFitCompents(){
  cout << "FitTime2Muon InitRooFitCompents ..." << endl ;

  m_muonRate_R = new RooRealVar("R_{#mu}", "R_{#mu}", 20., 0, 50);
  m_muonSuperScript_R = new RooFormulaVar("superScript_muon", "-1.e-3*@0", RooArgList(*m_muonRate_R));
  m_muonExp_R = new RooExponential("exp_muon", "background pdf for accidental coincidences", *m_x, *m_muonSuperScript_R); 
  m_muonNum_R = new RooRealVar("N_{bkg}", "number of accidental coincidences", 1.7e5, 0., 1.e8);

  for(int ielem=0; ielem<m_nElem; ielem++){
    RooRealVar *tau = new RooRealVar(m_elemTexName[ielem].Data(), m_elemTexName[ielem].Data(), 
        m_elemTau[ielem], m_elemTau[ielem]/2, m_elemTau[ielem]*2);
    m_elemTau_R.push_back(tau);

    TString tmp;
    tmp = Form("superScript_%s", m_elemName[ielem].Data());
    RooFormulaVar *superScript = new RooFormulaVar(tmp.Data(), "-1./@0", RooArgList(*tau));
    m_elemSuperScript_R.push_back(superScript);

    tmp = Form("exp_%s", m_elemName[ielem].Data());
    RooExponential *exp = new RooExponential(tmp.Data(), tmp.Data(), *m_x, *superScript);
    m_elemExp_R.push_back(exp);

    RooRealVar *num = new RooRealVar(m_elemNum[ielem].Data(), m_elemNum[ielem].Data(), 3.e4, 1., 1.e8);
    m_elemNum_R.push_back(num);

    tmp = Form("superScript_%s_muon", m_elemName[ielem].Data());
    RooFormulaVar *superScript_comb = new RooFormulaVar(tmp.Data(), "-1./@0 + (1.)*@1", RooArgList(*tau, *m_muonSuperScript_R));
    m_combinedSuperScript_R.push_back(superScript_comb);

    tmp = Form("exp_%s_muon", m_elemName[ielem].Data());
    RooExponential *exp_comb = new RooExponential(tmp.Data(), tmp.Data(), *m_x, *superScript_comb);
    m_combinedExp_R.push_back(exp_comb);
  }
}
