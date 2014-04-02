const int m_NAD=2;
//TString m_finputname="/publicfs/dyb/data/userdata/zhangfh/IbdAna/ana/P12C-bak/merged/mergedHistEH1.root";
//TString m_finputname="/publicfs/dyb/data/userdata/zhangfh/IbdAna/script/fastNeu/P12C/EH1/fn_EH1.root";
TString m_finputname="/afs/ihep.ac.cn/users/l/lidj/file/IBDSel/job/EH1Ibd.root";
TString m_foutputname="fn_EH1.root";

void fastNeu1(){
  TFile* finput=new TFile(m_finputname,"read");

  // Define energy bin
  double rnorm = 1.;
  const int nBins = 200;
  double xlow = 0.001, xup = 200;
  double eBinWt[nBins+1] = {0.0};
  double eBins[nBins+1] = {0.0};
  double eBinWidth = (TMath::Log10(xup)-TMath::Log10(xlow))/nBins;

  for(int i=1; i<=nBins; i++) {
    eBins[i] = xlow*TMath::Power(10, eBinWidth*i);
    eBinWt[i] = rnorm/(eBins[i]-eBins[i-1]);
  }

//  TH1F* promptE=new TH1F("promptE","",nBins,eBins);

  TCanvas* c1 = new TCanvas("c1","c1",800,600);
  TCanvas* c2 = new TCanvas("c2","c2",800,600);
/*
  for(Int_t i=1;i<=m_NAD;i++){
    TString histName;
    TString histTitle;

    histName = Form("zhangfh/allPromptEnergyAD%i",i);
    promptE->Add((TH1F*)gDirectory->Get(histName));
  }
*/
  //TH1F *promptE=finput->Get("promptE");
  TH1F *promptE=finput->Get("tFnProEWithoutrpc");
  for(int j=1; j<=nBins; j++) {
    double nEvt = promptE->GetBinContent(j);
    promptE->SetBinError(j, sqrt(nEvt*eBinWt[j]));
  }

  c1->cd();
  c1->SetLogy();
  promptE->GetXaxis()->SetTitle("Prompt energy(MeV)");
  promptE->GetYaxis()->SetTitle("Entries/1MeV");
  promptE->Draw("E");

  TH1F* promptEcopy=(TH1F*)promptE->Clone();

  c2->cd();
  c2->SetLogy();
  promptEcopy->GetXaxis()->SetTitle("MeV");
  promptEcopy->Draw("E");

  int minbin=promptE->FindBin(12);
  int maxbin=promptE->FindBin(100);
  double count=promptE->Integral(minbin,maxbin,"width");
  double unicount=count*(12-0.7)/(100-12);
  double unicounterr=sqrt(count)*(12-0.7)/(100-12);

  TF1* fPol1=new TF1("fPol1", "[0]+[1]*x", 12, 100);
  promptEcopy->Fit("fPol1","R");
  double pol1p0=fPol1->GetParameter(0);
  double pol1p0err=fPol1->GetParError(0);
  double pol1p1=fPol1->GetParameter(1);
  double pol1p1err=fPol1->GetParError(1);

  cout << "======== fit result ========" << endl;
  cout << "Uniform extrapolation, fast neutron: "<<unicount
       << ", error: "<<unicounterr<<endl;
  cout << "pol1, p0: "<<pol1p0<<", error: "<<pol1p0err<<endl;
  cout << "      p1: "<<pol1p1<<", error: "<<pol1p1err<<endl;
  cout << "fast neutron: "<<(pol1p0*2+pol1p1*(0.7+12))*(12-0.7)/2.
       << ", error: "<<(pol1p0err*2+pol1p1err*(0.7+12))*(12-0.7)/2.<<endl;

  TFile* foutput=new TFile(m_foutputname, "recreate");
  promptE->Write();
  c1->Write();
  c2->Write();
  foutput->Close();
  finput->Close();
}
