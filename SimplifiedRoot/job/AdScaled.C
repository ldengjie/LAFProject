#define AdScaled_cxx

#include "AdScaled.h"
#include <TH2.h>
#include <TStyle.h>
#include  <TTree.h>
TString rootName;
TTree* t1;
TFile* f1;
void AdScaled::Begin(TTree * /*tree*/)
{

   TString option = GetOption();
   rootName=option;
   f1=new TFile(rootName,"RECREATE");
   gDirectory->mkdir("Event");
   gDirectory->cd("Event");
   t1=new TTree("Simple","Simple of Event");
	
   t1->Branch("site", &site, "site/S");
   t1->Branch("detector", &detector, "detector/S");
   t1->Branch("triggerType", &triggerType, "triggerType/i");
   t1->Branch("triggerNumber", &triggerNumber, "triggerNumber/i");
   t1->Branch("triggerTimeSec", &triggerTimeSec, "triggerTimeSec/I");
   t1->Branch("triggerTimeNanoSec", &triggerTimeNanoSec, "triggerTimeNanoSec/I");
   t1->Branch("energy", &energy, "energy/F");
   t1->Branch("rawEvis", &rawEvis, "rawEvis/F");
   t1->Branch("enrec", &enrec, "enrec/F");
   t1->Branch("eprec", &eprec, "eprec/F");
   t1->Branch("x", &x, "x/F");
   t1->Branch("y", &y, "y/F");
   t1->Branch("z", &z, "z/F");
	
}

void AdScaled::SlaveBegin(TTree * /*tree*/)
{

   TString option = GetOption();

}

Bool_t AdScaled::Process(Long64_t entry)
{
   GetEntry(entry);
   t1->Write();

   return kTRUE;
}

void AdScaled::SlaveTerminate()
{

}

void AdScaled::Terminate()
{
	f1->Close();
}
