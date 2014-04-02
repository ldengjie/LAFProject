{
      TFile* f = TFile::Open("saveCanvasWithColor.root","RECREATE");
        TH1F* h = new TH1F("h","h",100,-5,5);
          h->FillRandom("gaus");
            h->SetLineColor(kRed);
              h->SetFillColor(kGreen+3);
                TCanvas* c = new TCanvas("c","c",800,600);
                  h->Draw("hist");
                    c->Write();
}
