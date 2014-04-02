// How to run
// root -b -q -l fitOff.cxx

void off1(TString site)
{
  // set input files, number of ADs, histogram colors for each hall
  const Int_t ndiv = 5;
  Int_t NumADs = 2;
  Int_t color = kBlack;
  TString strfile = site;
  if(site=="EH1") {
    NumADs = 2;
    strfile = "eh1_run_total.root";
  } else if(site=="EH2") {
    NumADs = 1;
    strfile = "eh2_run_total.root";
    color = kRed;
  } else if(site=="EH3") {
    NumADs = 3;
    strfile = "eh3_run_total.root";
    color = kBlue;
  }

  TFile f(strfile);
  // off-window tree
  offw=(TTree*)f.Get("hem/tree/offw");
  TH1F *h[4];
  for(Int_t i=0;i<NumADs;i++) {
    TString strdet = "";
    strdet += i+1;
    cout << "\n---" + site + "AD" + strdet + "---" << endl;

    c1 = new TCanvas();
    c1->SetTitle("");

    // define a histogram for accidentals against off-windows
    h[i] = new TH1F("h"+strdet,"",95,1.,20.); //1~20ms
    offw->Draw("dt>>h"+strdet,"ioffw>4&&det=="+strdet,"e"); //ioffw=0~4 means 0~1ms
    h[i]->SetMarkerStyle(24);
    h[i]->SetMarkerColor(color);
    h[i]->SetMarkerSize(1.2);
    h[i]->SetLineColor(color);
    h[i]->GetXaxis()->SetTitle("#DeltaT_{d-p}(ms)");
    h[i]->GetYaxis()->SetTitle("Event");
    h[i]->GetYaxis()->SetNdivisions(5);

    // fit with pol0
    TF1 *fit = new TF1("fit"+strdet,"pol0",1.,20.);
    h[i]->Fit("fit"+strdet);

    // get fitting paramters
    Double_t noff = fit->GetParameter(0);
    Double_t nofferr = fit->GetParError(0);
    cout << "noff=" << noff << ",err=" << nofferr << endl;

    // save fitting plots
    //h[i]->GetYaxis()->SetRangeUser(noff*0.92,noff*1.16);
    h[i]->GetYaxis()->SetRangeUser(noff-nofferr*40,noff+nofferr*80);
    h[i]->Draw("e");
    fit->Draw("same");
    c1->SaveAs("Off1" + site + "AD" + strdet + ".eps");
    c1->SaveAs("Off1" + site + "AD" + strdet + ".png");

    // get IBD candidate
    //ibd=(TH1F*)f.Get("hem/IBD/AD"+strdet+"IbdNum");
    //cout << "IBD candidate: " << ibd->Integral() << endl;

    // get neutron like correlated events for correction
    nc=(TH1F*)f.Get("hem/Singles/OffWinAD"+strdet+"nc");
    cout << "Neutron like correlated events : " << nc->Integral() << endl;

    // get neutron like singles for correction
    ns=(TH1F*)f.Get("hem/Singles/OffWinAD"+strdet+"ns");
    cout << "Neutron like singles: " << ns->Integral() << endl;

    // correction for accidental background
    Double_t nacc = noff*(ns->Integral()-nc->Integral()) / (ns->Integral()-noff);
    Double_t naccerr = (nofferr/noff)*nacc;
    cout << "nacc=" << nacc << ",err=" << naccerr << endl;

    // accidental background rate
    livetime=(TH1F*)f.Get("LiveTime/AD"+strdet+"LiveTime");
    Double_t acc_rate = 86400.*nacc/livetime->Integral();
    Double_t acc_rate_err = 86400.*naccerr/livetime->Integral();
    cout << "acc_rate=" << acc_rate << "/day,err=" << acc_rate_err << "/day" << endl;
  }
}

void off2(TString site)
{
  // set input files, number of ADs, histogram colors for each hall
  const Int_t ndiv = 5;
  Int_t NumADs = 2;
  Int_t color = kBlack;
  TString strfile = site;
  if(site=="EH1") {
    NumADs = 2;
    strfile = "eh1_run_total.root";
  } else if(site=="EH2") {
    NumADs = 1;
    strfile = "eh2_run_total.root";
    color = kRed;
  } else if(site=="EH3") {
    NumADs = 3;
    strfile = "eh3_run_total.root";
    color = kBlue;
  }

  TFile f(strfile);
  // off-window tree
  offw=(TTree*)f.Get("hem/tree/offw");
  TH1F *h[4];
  for(Int_t i=0;i<NumADs;i++) {
    TString strdet = "";
    strdet += i+1;
    cout << "\n---" + site + "AD" + strdet + "---" << endl;

    c1 = new TCanvas();
    c1->SetTitle("");

    // define a histogram for accidentals against off-windows
    h[i] = new TH1F("h"+strdet,"",95,1.,20.); //1~20ms
    offw->Draw("dt>>h"+strdet,"method==2&&ioffw>4&&det=="+strdet,"e"); //ioffw=0~4 means 0~1ms
    h[i]->SetMarkerStyle(24);
    h[i]->SetMarkerColor(color);
    h[i]->SetMarkerSize(1.2);
    h[i]->SetLineColor(color);
    h[i]->GetXaxis()->SetTitle("#DeltaT_{d-p}(ms)");
    h[i]->GetYaxis()->SetTitle("Event");
    h[i]->GetYaxis()->SetNdivisions(5);

    // fit with pol0
    TF1 *fit = new TF1("fit"+strdet,"pol0",1.,20.);
    h[i]->Fit("fit"+strdet);

    // get fitting paramters
    Double_t noff = fit->GetParameter(0);
    Double_t nofferr = fit->GetParError(0);
    cout << "noff=" << noff << ",err=" << nofferr << endl;

    // save fitting plots
    //h[i]->GetYaxis()->SetRangeUser(noff*0.92,noff*1.16);
    h[i]->GetYaxis()->SetRangeUser(noff-nofferr*40,noff+nofferr*80);
    h[i]->Draw("e");
    fit->Draw("same");
    c1->SaveAs("Off2" + site + "AD" + strdet + ".eps");
    c1->SaveAs("Off2" + site + "AD" + strdet + ".png");

    // get positron like singles for efficiency correction
    ps=(TH1F*)f.Get("hem/Singles/AD"+strdet+"ps");
    cout << "Positron like singles: " << ps->Integral() << endl;

    // get live time
    livetime=(TH1F*)f.Get("LiveTime/AD"+strdet+"LiveTime");
    cout << "Live time: " << livetime->Integral()/86400. << endl;

    // caculate positron like single rate
    Double_t ps_rate = ps->Integral()/livetime->Integral();
    ps_rate = ps_rate/(TMath::Exp(-ps_rate*4.e-4)); // correction for isolated cut
    cout << "Positron like single rate: " << ps_rate << "Hz" << endl;

    // correction for accidental backgrounds
    Double_t nacc = noff/TMath::Exp(-2.e-4*ps_rate);
    Double_t naccerr = nofferr/TMath::Exp(-2e-4*ps_rate);
    cout << "nacc=" << nacc << ",err=" << naccerr << endl;

    // accidental background rate
    Double_t acc_rate = 86400.*nacc/livetime->Integral();
    Double_t acc_rate_err = 86400.*naccerr/livetime->Integral();
    cout << "acc_rate=" << acc_rate << "/day,err=" << acc_rate_err << "/day" << endl;

    // get neutron like singles for theoretical calculation
    ns=(TH1F*)f.Get("hem/Singles/AD"+strdet+"ns");
    //cout << "Neutron like singles: " << ns->Integral() << endl;

    // theoretical calculation for accidental backgrounds
    Double_t the_nacc = ns->Integral()*(1-TMath::Exp(-ps_rate*2.e-4));
    Double_t the_acc_rate = 86400.*the_nacc/livetime->Integral();
    Double_t the_acc_rate_err = the_acc_rate/TMath::Sqrt(ns->Integral());
    cout << "theoretical calculated acc_rate=" << the_acc_rate 
      << "/day,err=" << the_acc_rate_err << "/day" << endl;

  }
}

void draw(TString site)
{
  site = "EH2";
  cout << "==== " << site << "====" << endl;
  Int_t NumADs = 2;
  Int_t color = kBlack;
  TString strfile = site;
  if(site=="EH1") {
    NumADs = 2;
    strfile = "eh1_run_total.root";
  } else if(site=="EH2") {
    NumADs = 1;
    strfile = "eh2_run_total.root";
    color = kRed;
  } else if(site=="EH3") {
    NumADs = 3;
    strfile = "eh3_run_total.root";
    color = kBlue;
  }

  //NumADs = 1;
  TFile f(strfile);
  offw=(TTree*)f.Get("hem/tree/offw");
  TH1F *h[4];
  for(Int_t i=0;i<NumADs;i++) {
    c1 = new TCanvas();
    c1->SetTitle("");
    TString strcut = "method==2&&ioffw>4&&det==";
    strcut += i+1;
    TString strh = "h";
    strh += i+1;
    TString strdraw = "dt>>";
    strdraw += strh;
    h[i] = new TH1F(strh,strh,95,1.,20.);
    offw->Draw(strdraw,strcut,"e");
    h[i]->SetTitle("");
    h[i]->SetMarkerStyle(24);
    h[i]->SetMarkerColor(color);
    h[i]->SetMarkerSize(1.2);
    h[i]->SetLineColor(color);
    h[i]->GetXaxis()->SetTitle("#DeltaT_{d-p}(ms)");
    h[i]->GetYaxis()->SetTitle("Event");
    c1->Clear();
    h[i]->Draw("e");
    h[i]->Fit("pol0");
    //TString strpic = site;
    //strpic += "AD";
    //strpic += i+1;
    //strpic += ".eps";
    //c1->SaveAs(strpic);
  }
  f.Close();
}

void fitOff()
{
  cout << "Off-window method 1:" << endl;
  off1("EH1");
  off1("EH2");
  off1("EH3");
  cout << "\n\nOff-window method 2:" << endl;
  off2("EH1");
  off2("EH2");
  off2("EH3");
}
