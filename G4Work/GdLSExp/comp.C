{
   TH1F* hdE[2];
   TH1F* hdEdx[2];
   TH2D* dEdx2D[2];

   //TFile* file1 = new TFile("positron1MeV_noAnni_default.root");
   //TFile* file1 = new TFile("electron1MeV_noAnni_default.root");
   //TFile* file1 = new TFile("gamma1MeV_default.root");
   TFile* file1 = new TFile("electron3MeV_Penelope_noPosiAnni.root");
   hdE[0] = (TH1F*) file1->Get("electrondE");
   hdEdx[0] = (TH1F*) file1->Get("electrondEdx");
   dEdx2D[0] = (TH2D*) file1->Get("dEdx2D");

   //TFile* file2 = new TFile("positron1MeV_noAnni_Penelope.root");
   //TFile* file2 = new TFile("electron1MeV_noAnni_Penelope.root");
   //TFile* file2 = new TFile("gamma1MeV_Penelope.root");
   TFile* file2 = new TFile("positron3MeV_Penelope_noPosiAnni.root");
   hdE[1] = (TH1F*) file2->Get("electrondE");
   hdEdx[1] = (TH1F*) file2->Get("electrondEdx");
   dEdx2D[1] = (TH2D*) file2->Get("dEdx2D");

   TCanvas* c1 = new TCanvas("c1", "c1", 500, 500);
   hdE[1]->Draw();
   hdE[1]->SetLineColor(2); hdE[0]->Draw("same");
   cout << "default dE sum  : " << hdE[0]->GetMean(1)*hdE[0]->GetEntries()/5000 << endl;
   cout << "Penelope dE sum : " << hdE[1]->GetMean(1)*hdE[1]->GetEntries()/5000 << endl;
   hdE[0]->SetLineWidth(2);
   hdE[1]->SetLineWidth(2);

   TCanvas* c2 = new TCanvas("c2", "c2", 500, 500);
   hdEdx[1]->Draw();
   hdEdx[1]->SetLineColor(2); hdEdx[0]->Draw("same");
   hdEdx[0]->SetLineWidth(2);
   hdEdx[1]->SetLineWidth(2);

   TCanvas* c3 = new TCanvas("c3", "c3", 600, 600);
   dEdx2D[0]->Draw("COLZ");
   dEdx2D[0]->GetXaxis()->SetTitle("dE");
   dEdx2D[0]->GetYaxis()->SetTitle("dx");
   c3->SetLogz(1);

   TCanvas* c4 = new TCanvas("c4", "c4", 600, 600);
   dEdx2D[1]->Draw("COLZ");
   dEdx2D[1]->GetXaxis()->SetTitle("dE");
   dEdx2D[1]->GetYaxis()->SetTitle("dx");
   c4->SetLogz(1);
}
