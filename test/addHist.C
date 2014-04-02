{
	TFile* f=new TFile("FastNeu_HistEH1_OWS_Fast_neutron_AdSimple.root");
	TH1F* h1=(TH1F*)f->Get("ProEAD1");
	TH1F* h2=(TH1F*)f->Get("ProEAD2");
	TH1F* h3=new TH1F(*h1);
	h3->Add(h1,h2,1,1);
	//h3->Draw();
	TFile* f1=new TFile("addHist.root","recreate");
	h3->Write();
	f1->Close();
}
