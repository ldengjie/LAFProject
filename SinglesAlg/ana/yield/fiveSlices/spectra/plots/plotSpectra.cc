TString site = "EH1";
void plotSpectra(){
  TFile* f = new TFile("../Hist_EHs_B12.root", "read");

  const int eMuonMinArr[] = {20, 60, 500, 1500, 2500, 5000};
  const TString strDirs[] = {"Muon20to60", "Muon60to500", "Muon500to1500", "Muon1500to2500", "Muon2500to5000"};
  const int nSlices = 6-1; //sizeof(eMuonMinArr)/sizeof(int);

  int nAds = 0;
  if(site=="EH1") nAds = 2;
  if(site=="EH2") nAds = 2;
  if(site=="EH3") nAds = 4;
  TDirectory* siteDir = (TDirectory*)f->GetDirectory(site);
  TDirectory* reductionDirs[2];
  reductionDirs[0] = (TDirectory*)siteDir->GetDirectory("noReduction");
  reductionDirs[1] = (TDirectory*)siteDir->GetDirectory("afterReduction");
  TDirectory* sliceDirs[2][10];
  for(int itype=0; itype<2; itype++){
    for(int islice=0; islice<nSlices; islice++){
      sliceDirs[itype][islice] = (TDirectory*)reductionDirs[itype]->GetDirectory(strDirs[islice]);
    }
  }

  TString title;
  TH1F* h_spec_noReduction[nSlices][4];
  TH1F* h_spec_afterReduction[nSlices][4];

  TH1F* h_spec_noReduction_ads[nSlices];
  TH1F* h_spec_afterReduction_ads[nSlices];
  for(int islice=0; islice<nSlices; islice++){
    int eMuonLow  = eMuonMinArr[islice];
    int eMuonHigh = eMuonMinArr[islice + 1];
    for(int iad=0; iad<nAds; iad++){
      title = Form("eSpec_noReduction_ad%d_Muon%dto%d", iad+1, eMuonLow, eMuonHigh);
      h_spec_noReduction[islice][iad] = (TH1F*)sliceDirs[0][islice]->Get(title);
      drawHist(h_spec_noReduction[islice][iad]);

      title = Form("eSpec_afterReduction_ad%d_Muon%dto%d", iad+1, eMuonLow, eMuonHigh);
      h_spec_afterReduction[islice][iad] = (TH1F*)sliceDirs[1][islice]->Get(title);
      drawHist(h_spec_afterReduction[islice][iad]);
    }
    title = Form("eSpec_noReduction_ads_Muon%dto%d", eMuonLow, eMuonHigh);
    h_spec_noReduction_ads[islice] = (TH1F*)sliceDirs[0][islice]->Get(title);
    drawHist(h_spec_noReduction_ads[islice]);

    title = Form("eSpec_afterReduction_ads_Muon%dto%d", eMuonLow, eMuonHigh);
    h_spec_afterReduction_ads[islice] = (TH1F*)sliceDirs[1][islice]->Get(title);
    drawHist(h_spec_afterReduction_ads[islice]);
  }
}
void drawHist(TH1F* h){
  int binL = h->FindBin(6);
  int binH = h->FindBin(9);
  double valMax = 0;
  for(int ibin=binL; ibin<=binH; ibin++){
    double val = h->GetBinContent(ibin);
    if(val > valMax) valMax = val;
  }
  int yMin = 0;
  //int yMin = 0-int(0.05*valMax);
  int yMax = int(valMax*1.1);
  h->GetYaxis()->SetRangeUser(yMin, yMax);
  h->GetXaxis()->SetTitle("Reconstructed energy [MeV]");
  h->GetXaxis()->SetRangeUser(0, 20);
  TCanvas *c = new TCanvas("c", "c", 900, 600);
  h->Draw();
  TString title = Form("%s/%s.png", site.Data(), h->GetName());
  c->Print(title, "png");
  delete c;
}
