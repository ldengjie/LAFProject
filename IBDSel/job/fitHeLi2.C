{
  gStyle->SetOptStat(0);
  gSystem->Load("libRooFit");
  gSystem->Load("libCintex");
  using namespace RooFit;

  const int nhist = 6;
  int nrebin = 10;
  TString filename = "/publicfs/dyb/data/userdata/zhangfh/IbdAna/ana/HeLiAna/P12Etot/EH2/mergedHist.root";
  TString Site = "EH2";
  TString outputname="EH2all.root";
  int nAD=1;

  double showerTh[nhist+1] = {0.02, 0.5, 1.5, 2.5, 3.5, 4.5, 5.0};
  TString strShowerTh[nhist] = {"0.02to0.5", "0.5to1.5", "1.5to2.5", "2.5to3.5", "3.5to4.5", "gt4.5"};
  TH1D* result = new TH1D("result", "result", nhist, showerTh); 

  TH1D* t2lastMuSliceMerge[nhist];

  TFile* file = new TFile(filename,"read");
  TFile* foutput=new TFile(outputname,"recreate");
  file->cd();

  TCanvas* c3 = new TCanvas("c3", "c3", 600, 400);
  double x_min = 0.001;
  double x_max = 40.0;
  RooRealVar   x("x","x",x_min,x_max, "s");

  RooRealVar tauLi9("#tau_{^{9}Li}", "#tau_{^{9}Li}", -0.257, "s");
  RooRealVar tauHe8("#tau_{^{8}He}", "#tau_{^{8}He}", -0.172, "s");
  
  RooRealVar tauMu("#tau_{ibd}", "#tau_{ibd}", -1.0, -1000., 0., "s");
  
  RooFormulaVar lambdaLi9("#lambda_{^{9}Li}", "#lambda_{^9Li}", "1/@0 + 1/@1", RooArgList(tauLi9, tauMu));
  RooFormulaVar lambdaHe8("#lambda_{^{8}He}", "#lambda_{^8He}", "1/@0 + 1/@1", RooArgList(tauHe8, tauMu));
  
  RooFormulaVar lambdaMu("#lambda_{#mu}", "#lambda_{#mu}", "1/@0", RooArgList(tauMu));

  RooExponential expLi9("expLi9", "^{9}Li distribution", x, lambdaLi9);
  RooExponential expHe8("expHe8", "^{8}He distribution", x, lambdaHe8);
  
  RooExponential expMu("expMu", "^{8}Li distribution", x, lambdaMu);

  RooRealVar nsigHe8Li9("N_{ ^{9}Li}+N_{ ^{8}He}","N_{ ^{9}Li}+N_{ ^{8}He}", 1.e1, 0.0, 2.e3);
  RooRealVar nsigMu("N_{ibd}","N_{ibd}", 3e2, 1e1, 2.e5);

  RooRealVar Li9Frac("Li9Frac","Li9Frac", 0.0);

  RooFitResult* fitres;
  RooFormulaVar nsigLi9, nsigHe8;
  RooAddPdf* sum;
  RooDataHist* data;
  double minNLL[41] = {0.};
  double li9[41] = {0.}, li9Err[41] = {0.}, li9Frac[41] = {0.};
  double glbminNLL = 0.; int minIndex = 0;

  // Fit results
  double li9Fit[nhist]={0.}, li9FitErr[nhist]={0.};
  double MuRateFit[nhist]={0.}, MuRateFitErr[nhist]={0.};
  double showerThErr[nhist] = {0.};
  double li9ratio[nhist]={0.};

  for(int ihist=1; ihist<=nhist; ihist++){
    //TString str = "t2lastMu"; str += ihist;
    TString str = "zhangfh/time2LastMuonAll"; 
    /*
    switch(ihist){
    case 1:str += "500"; str+=Ad; break;
    case 2:str += "500to1k5"; str+=Ad; break;
    case 3:str += "1k5to2k5"; str+=Ad; break;
    case 4:str += "2k5to3k5"; str+=Ad; break;
    case 5:str += "3k5to4k5"; str+=Ad; break;
    case 6:str += "4k5"; str+=Ad; break;
    }
    */
    str+=ihist;
    //cout << "0" << endl;
    t2lastMuSliceMerge[ihist-1] = (TH1D*) file->Get(str);
    //cout << "1" << endl;
    t2lastMuSliceMerge[ihist-1]->Rebin(nrebin);
    // RooFit data set
    data = new RooDataHist("data", "data", x, t2lastMuSliceMerge[ihist-1]);
    //cout << "2" << endl;
    // scan the fraction of Li9
    for(int i=0; i<41; i++) {
      Li9Frac.setVal(i*0.025);
      RooFormulaVar nsigHe8("He", "He", "@0 * ( 1 - @1 )", RooArgList(nsigHe8Li9, Li9Frac));
      RooFormulaVar nsigLi9("Li", "Li", "@0 * @1", RooArgList(nsigHe8Li9, Li9Frac));
      RooRealVar nsigMu("N", "N", 3e2, 1e1, 2.e5);
      RooAddPdf* sum  = new RooAddPdf("sum","",RooArgList(expLi9, expHe8, expMu),RooArgList(nsigLi9, nsigHe8, nsigMu));
      cout << "1" << endl;
      fitres = sum->fitTo((*data), Save());
      cout << Form("%8.8f", fitres->minNll()) << endl;

      li9[i] = nsigHe8Li9.getVal(0);
      li9Err[i] = nsigHe8Li9.getError();
      li9Frac[i] = nsigLi9.getVal(0)/(nsigLi9.getVal(0) + nsigHe8.getVal(0));
      minNLL[i] = fitres->minNll();
      if(minNLL[i]<glbminNLL) { glbminNLL = minNLL[i]; minIndex = i; }
    }
    for(int i=0; i<41; i++) {
      cout << Form("%8.8f", minNLL[i]) << "    " << li9[i] << "    " << li9Err[i] << "    " << li9Frac[i] << endl;
    }
    cout << "minIndex: " << minIndex << endl;
    cout << Form("%8.8f", minNLL[minIndex]) << "    " << li9[minIndex] << "    " << li9Err[minIndex] << "    " << li9Frac[minIndex] << endl;
    
    // find the minimum -Log(likelihood)
    Li9Frac.setVal( minIndex * 0.025);
    RooFormulaVar nsigHe8("He", "He", "@0 * ( 1 - @1 )", RooArgList(nsigHe8Li9, Li9Frac));
    RooFormulaVar nsigLi9("Li", "Li", "@0 * @1", RooArgList(nsigHe8Li9, Li9Frac));
    RooRealVar nsigMu("N_{ibd}","N_{ibd}", 3e2, 1e1, 2.e5);
    
    sum  = new RooAddPdf("sum","",RooArgList(expLi9, expHe8, expMu),RooArgList(nsigLi9, nsigHe8, nsigMu));
    fitres = sum->fitTo((*data), Save());
    cout << Form("%8.8f", fitres->minNll()) << endl;
    cout <<  nsigHe8Li9.getVal(0) << "     " << nsigHe8Li9.getError() 
	 << "     " << nsigLi9.getVal(0)/(nsigLi9.getVal(0) + nsigHe8.getVal(0)) << endl;
    li9Fit[ihist-1] = nsigHe8Li9.getVal(0);
    li9FitErr[ihist-1] = nsigHe8Li9.getError();
    li9ratio[ihist-1] = nsigLi9.getVal(0)/(nsigLi9.getVal(0) + nsigHe8.getVal(0));

    RooPlot* xframe = x.frame(Title(" "));
    //xframe->SetAxisRange(0.1, 8.0, "X");
    data->plotOn(xframe);
    sum->plotOn(xframe);
    sum->paramOn(xframe, Label(""), Format("NEU",AutoPrecision(1)));
    xframe->Draw();
    xframe->GetXaxis()->SetTitle("time since last muon (s)");
    xframe->GetYaxis()->SetTitle("Entries");
    xframe->GetYaxis()->SetRangeUser(0.1,10000);
    //xframe->GetYaxis()->SetRangeUser(10, 4000);
    
    c3->SetLogy();
    //c3->Print("plots/Li9Fit/gt4.0GeV_0to20s_0.1sBin_singleLi9.eps", ".eps");
    //c3->Print("plots/" + Site +"/Li9Fit_"+ strShowerTh[ihist-1] + "GeV_oldRed30s.png", "png");
    
    foutput->cd();
    c3->Write("Li9Fit_"+ strShowerTh[ihist-1] + "GeV");
    file->cd();

    result->SetBinContent(ihist, li9Fit[ihist-1]);
    result->SetBinError(ihist, li9FitErr[ihist-1]);
  }

  TCanvas* c4 = new TCanvas("c4", "c4", 600, 400);
  gStyle->SetEndErrorSize(0.0);
  result->SetMarkerStyle(20);
  result->SetMarkerColor(4);
  result->SetLineColor(4);
  result->SetMarkerSize(1.0);
  result->Draw("EP");
  result->SetMinimum(0);
  result->SetMaximum(600);
  result->GetXaxis()->SetTitle("Muon visible energy (GeV)");
  result->GetYaxis()->SetTitle("Fitted ^{9}Li events");
  result->SetTitle("");
  //c4->Print("plots/" + Site + "/Li9Fit_muonTh_oldRed30s.png", "png");
  //c4->Print("plots/" + Site + "/Li9Fit_muonTh_oldRed30s.eps", "eps");
  foutput->cd();
  c4->Write("Li9Fit_muonTh");
  result->Write();

  cout << "Li9 fit retuls: " << endl;
  for(int ihist=0; ihist<nhist; ihist++) cout << Form("%4.2f", li9Fit[ihist]) << ", ";
  cout << endl;
  for(int ihist=0; ihist<nhist; ihist++) cout << Form("%4.2f", li9FitErr[ihist]) << ", ";
  cout << endl;
  for(int ihist=0; ihist<nhist; ihist++) cout << li9ratio[ihist] << ", ";
  cout << endl;

  file->cd();
  TH1D* adMuSliceRate[nhist];
  TH1D* adMuSliceRateAll[nhist];
  TH1D* resultRate = new TH1D("resultRate", "resultRate", nhist, showerTh); 
  TH1D* resultRateAll = new TH1D("resultRateAll", "resultRateAll",
				 nhist, showerTh); 
  TH1D* resultRatio = new TH1D("resultRatio", "resultRatio", nhist, showerTh);
  TH1D* resultRatioAll = new TH1D("resultRatioAll", "resultRatioAll",
				  nhist, showerTh);
  TH1D* daqLiveTime = (TH1D*)file->Get("zhangfh/daqLiveTime"); 
  for(int ihist=1; ihist<=nhist; ihist++){
    TString str = "zhangfh/adMuonRateRed"; 
    str+=ihist;
    TString strall = "zhangfh/adMuonRateAll"; 
    strall+=ihist;

    for(int i=1;i<=nAD;i++){
      TString stri=str;
      stri+="AD";
      stri+=i;
      if(i==1){
      adMuSliceRate[ihist-1] = (TH1D*)file->Get(stri);
      }
      else {
	adMuSliceRate[ihist-1]->Add((TH1D*)file->Get(stri));
      }

      TString striall=strall;
      striall+="AD";
      striall+=i;
      if(i==1){
      adMuSliceRateAll[ihist-1] = (TH1D*)file->Get(striall);
      }
      else {
	adMuSliceRateAll[ihist-1]->Add((TH1D*)file->Get(striall));
      }
    }
    double muonCount=adMuSliceRate[ihist-1]->Integral();
    double totaltime=daqLiveTime->Integral()*nAD;
    resultRate->SetBinContent(ihist, muonCount/totaltime);
    resultRate->SetBinError(ihist, sqrt(muonCount)/totaltime);

    double muonCountAll=adMuSliceRateAll[ihist-1]->Integral();
    resultRateAll->SetBinContent(ihist, muonCountAll/totaltime);
    resultRateAll->SetBinError(ihist, sqrt(muonCountAll)/totaltime);

    double sigF=li9FitErr[ihist-1];
    double sigM=sqrt(muonCount);
    double drdF = 1/muonCount;
    double drdM = -li9Fit[ihist-1]/muonCount/muonCount;
    double ratioErr=sqrt(sigF*sigF*drdF*drdF+sigM*sigM*drdM*drdM);
    resultRatio->SetBinContent(ihist, li9Fit[ihist-1]/muonCount);
    resultRatio->SetBinError(ihist, ratioErr);

    double sigFAll=li9FitErr[ihist-1];
    double sigMAll=sqrt(muonCountAll);
    double drdFAll = 1/muonCountAll;
    double drdMAll = -li9Fit[ihist-1]/muonCountAll/muonCountAll;
    double ratioAllErr=sqrt(sigFAll*sigFAll*drdFAll*drdFAll
			    +sigMAll*sigMAll*drdMAll*drdMAll);
    resultRatioAll->SetBinContent(ihist, li9Fit[ihist-1]/muonCountAll);
    resultRatioAll->SetBinError(ihist, ratioAllErr);
    cout << li9Fit[ihist-1]/muonCountAll << " +- " << ratioAllErr << ", ";
  }

  foutput->cd();
  resultRate->Write();
  TCanvas* c5 = new TCanvas("c5", "c5", 600, 400);
  resultRatio->Draw("EP");
  resultRatio->Write();
  resultRateAll->Write();
  TCanvas* c6 = new TCanvas("c6", "c6", 600, 400);
  resultRatioAll->Draw("EP");
  resultRatioAll->Write();
}
