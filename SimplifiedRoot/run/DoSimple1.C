#include  <fstream>
#include  <string>
#include  <iostream>
using namespace std;
void DoSimple1()
{
//	gROOT->ProcessLine(".L AdScaled.C+");
	ifstream runlist;
	runlist.open("recon.list");
	string run;
	while( getline(runlist,run) )
	{
		cout<<"Now is processing "<<TString(run)<<endl;
		TFile *oldfile = new TFile(TString(run));
		TTree *oldtree = (TTree*)oldfile->Get("/Event/Rec/AdScaled");
		oldtree->SetBranchStatus("*",0); 
		oldtree->SetBranchStatus("site", 1);
		oldtree->SetBranchStatus("detector",1);
		oldtree->SetBranchStatus("triggerType",1);
		oldtree->SetBranchStatus("triggerNumber",1);
		oldtree->SetBranchStatus("triggerTimeSec",1);
		oldtree->SetBranchStatus("triggerTimeNanoSec",1);
		oldtree->SetBranchStatus("energy",1);
		oldtree->SetBranchStatus("rawEvis",1);
		oldtree->SetBranchStatus("enrec",1);
		oldtree->SetBranchStatus("eprec",1);
		oldtree->SetBranchStatus("x",1);
		oldtree->SetBranchStatus("y",1);
		oldtree->SetBranchStatus("z",1);

		TTree *oldtree1 = (TTree*)oldfile->Get("/Event/Data/CalibStats");
		oldtree1->SetBranchStatus("*",0); 
		oldtree1->SetBranchStatus("nHit", 1);
		oldtree1->SetBranchStatus("MaxQ",1);
		oldtree1->SetBranchStatus("Quadrant",1);
		oldtree1->SetBranchStatus("MaxQ_2inchPMT",1);
		oldtree1->SetBranchStatus("Column_2inchPMT",1);
		oldtree1->SetBranchStatus("MiddleTimeRMS",1);
		oldtree1->SetBranchStatus("tRMS",1);
		oldtree1->SetBranchStatus("nPEMax",1);
		oldtree1->SetBranchStatus("nPEMedian",1);
		oldtree1->SetBranchStatus("nPERMS",1);
		oldtree1->SetBranchStatus("nPESum",1);

		TTree *oldtree2 = (TTree*)oldfile->Get("/Event/CalibReadout/CalibReadoutHeader");
		oldtree2->SetBranchStatus("*",0); 
		oldtree2->SetBranchStatus("nHitsRpc", 1);
		oldtree2->SetBranchStatus("rpcRow", 1);
		oldtree2->SetBranchStatus("rpcColumn", 1);
		oldtree2->SetBranchStatus("rpcLayer", 1);

		TFile *newfile = new TFile("small.root","recreate");
		gDirectory->mkdir("Event");
		gDirectory->cd("Event");
		gDirectory->mkdir("Rec");
		gDirectory->mkdir("Data");
		gDirectory->mkdir("CalibReadout");
		gDirectory->cd("Rec");
		TTree *newtree = oldtree->CloneTree();
		gDirectory->cd("../Data");
		TTree *newtree1 = oldtree1->CloneTree();
		gDirectory->cd("../CalibReadout");
		TTree *newtree2 = oldtree2->CloneTree();
		newfile->Write(); 
		delete oldfile;
		delete newfile;
	}

}
