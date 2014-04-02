#include  <iostream>
#include  <TChain.h>
#include  <TTree.h>
#include  <TH1F.h>
#include  <TMath.h>
#include  <TFile.h>
int temp()
{
	TFile* f =new TFile("ibd.root","recreate");
	TChain* ibdTree = new TChain("ibdTree/ibdTree");
	ibdTree->Add("EH1/run*IBD.root");
	//TFile* f =new TFile("ibd.root","recreate");
	TH1F* PromptE = new TH1F("PromptE","The prompt energy spectra ",60,0,12);
	TH1F* DelayedE = new TH1F("DelayedE","The delayed energy spectra",30,6,12);
	TH1F* DeltaT = new TH1F("DeltaT","Time between the prompt and delayer signals",40,0,200); 
	TH1F* Distance = new TH1F("Distance","Distance between the prompt and delayed signals",30,0,3);
	Float_t Energy[2],X[2],Y[2],Z[2],deltaT,distance;
	ibdTree->SetBranchAddress("Energy",Energy);
	ibdTree->SetBranchAddress("X",X);
	ibdTree->SetBranchAddress("Y",Y);
	ibdTree->SetBranchAddress("Z",Z);
	ibdTree->SetBranchAddress("deltaT",&deltaT);
	Int_t IBDNum = ibdTree->GetEntries();
	//Int_t IBDNum=1000;
	for( Int_t i=0 ; i<IBDNum ; i++ )
	{
		ibdTree->GetEntry(i);
		distance=sqrt(pow((X[0]-X[1]),2)+pow((Y[0]-Y[1]),2)+pow((Z[0]-Z[1]),2));
		PromptE->Fill(Energy[0]);
		DelayedE->Fill(Energy[1]);
		DeltaT->Fill(deltaT);
		Distance->Fill(distance);

	}
	//PromptE->Draw();
	//DelayedE->Draw();
	//DeltaT->Draw();
	//Distance->Draw();
	f->Write();
}
