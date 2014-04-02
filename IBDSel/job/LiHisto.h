//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Oct 24 23:40:11 2013 by ROOT version 5.26/00e
// from TTree IbdTree/IbdTree
// found on file: /afs/ihep.ac.cn/users/l/lidj/largedata/IBD/P13A/EH2/run36961_IBD.root
//////////////////////////////////////////////////////////

#ifndef LiHisto_h
#define LiHisto_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TSelector.h>

class LiHisto : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   TString histname;
   TFile* file;
   TString dataVer;
   int ADNum;
   int site;
   TH1F* t2lastshowermuon[5][6];

   Int_t           isIbd;
   Int_t           isLi;
   Int_t           det;
   Float_t         energy[2];
   Float_t         x[2];
   Float_t         y[2];
   Float_t         z[2];
   Int_t           fileNum[2];
   Long64_t        entry[2];
   Long64_t        trigNum[2];
   float        timeInterval;
   Int_t           delayedTrigTimeSec;
   float         promptT2Muon[22];
   float         delayedT2Muon[22];

   // List of branches
   TBranch        *b_isIbd_l;   //!
   TBranch        *b_isLi_l;   //!
   TBranch        *b_det_l;   //!
   TBranch        *b_energy_l;   //!
   TBranch        *b_x_l;   //!
   TBranch        *b_y_l;   //!
   TBranch        *b_z_l;   //!
   TBranch        *b_fileNum;   //!
   TBranch        *b_entry;   //!
   TBranch        *b_trigNum;   //!
   TBranch        *b_timeInterval;   //!
   TBranch        *b_delayedTrigTimeSec;   //!
   TBranch        *b_promptT2Muon;   //!
   TBranch        *b_delayedT2Muon;   //!

   LiHisto(TTree * /*tree*/ =0) { }
   virtual ~LiHisto() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(LiHisto,0);
};

#endif

#ifdef LiHisto_cxx
void LiHisto::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("isIbd", &isIbd, &b_isIbd_l);
   fChain->SetBranchAddress("isLi", &isLi, &b_isLi_l);
   fChain->SetBranchAddress("det", &det, &b_det_l);
   fChain->SetBranchAddress("energy", energy, &b_energy_l);
   fChain->SetBranchAddress("x", x, &b_x_l);
   fChain->SetBranchAddress("y", y, &b_y_l);
   fChain->SetBranchAddress("z", z, &b_z_l);
   fChain->SetBranchAddress("fileNum", fileNum, &b_fileNum);
   fChain->SetBranchAddress("entry", entry, &b_entry);
   fChain->SetBranchAddress("trigNum", trigNum, &b_trigNum);
   fChain->SetBranchAddress("timeInterval", &timeInterval, &b_timeInterval);
   fChain->SetBranchAddress("delayedTrigTimeSec", &delayedTrigTimeSec, &b_delayedTrigTimeSec);
   fChain->SetBranchAddress("promptT2Muon", promptT2Muon, &b_promptT2Muon);
   fChain->SetBranchAddress("delayedT2Muon", delayedT2Muon, &b_delayedT2Muon);
}

Bool_t LiHisto::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef LiHisto_cxx
