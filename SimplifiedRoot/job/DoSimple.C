#include  <fstream>
#include  <string>
#include  <iostream>
using namespace std;
void DoSimple()
{
	gROOT->ProcessLine(".L AdScaled.C+");
	ifstream runlist;
	runlist.open("recon.list");
	string run;
	while( getline(runlist,run) )
	{
		cout<<"Now is processing "<<TString(run)<<endl;
		TFile *f = new TFile(TString(run));
		TTree *t = (TTree*)f->Get("/Event/Rec/AdScaled");
		t->Process("AdScaled",TString(run));
		delete f;
	}

}
