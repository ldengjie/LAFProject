{
  TFile* test = new TFile("test1.root", "recreate");
  TH1F* h1 = new TH1F("h1", "h1", 1000000, 0, 1);
  h1->Fill(0.1);
  h1->Write();
  test->Close();

  TFile* test = new TFile("test2.root", "recreate");
  TH1F* h2 = new TH1F("h2", "h2", 1000000, 0, 1);
  h2->Fill(0.2);
  h2->Write();
  test->Close();
}
