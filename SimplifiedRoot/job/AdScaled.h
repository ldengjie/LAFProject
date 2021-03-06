//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Aug  7 13:54:17 2012 by ROOT version 5.26/00e
// from TTree AdScaled/Tree at /Event/Rec/AdScaled holding Rec_AdScaled
// found on file: recon.root
//////////////////////////////////////////////////////////

#ifndef AdScaled_h
#define AdScaled_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
   const Int_t kMaxinputHeaders = 2;

class AdScaled : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
 //PerRecHeader    *Rec_AdScaled;
   Int_t           clID;
   Int_t           earliest_mSec;
   Int_t           earliest_mNanoSec;
   Int_t           latest_mSec;
   Int_t           latest_mNanoSec;
   Int_t           context_mSite;
   Int_t           context_mSimFlag;
   Int_t           context_mTimeStamp_mSec;
   Int_t           context_mTimeStamp_mNanoSec;
   Int_t           context_mDetId;
   Int_t           execNumber;
   UInt_t          jobId_m_data[4];
   vector<unsigned long> randomState;
   Int_t           inputHeaders_;
   Int_t           inputHeaders_m_entry[kMaxinputHeaders];   //[inputHeaders_]
   string          inputHeaders_m_path[kMaxinputHeaders];
   Short_t         site;
   Short_t         detector;
   UInt_t          triggerType;
   UInt_t          triggerNumber;
   Int_t           triggerTimeSec;
   Int_t           triggerTimeNanoSec;
   Float_t         energy;
   Float_t         rawEvis;
   Float_t         enrec;
   Float_t         eprec;
   Float_t         x;
   Float_t         y;
   Float_t         z;
   Float_t         t;
   Float_t         dx;
   Float_t         dy;
   Float_t         dz;
   Float_t         dt;
   Int_t           energyStatus;
   Int_t           positionStatus;
   Int_t           directionStatus;
   Float_t         energyQuality;
   Float_t         positionQuality;
   Float_t         directionQuality;
   Int_t           errorMatrixDim;
   vector<float>   errorMatrix;

   // List of branches
   TBranch        *b_Rec_AdScaled_clID;   //!
   TBranch        *b_Rec_AdScaled_earliest_mSec;   //!
   TBranch        *b_Rec_AdScaled_earliest_mNanoSec;   //!
   TBranch        *b_Rec_AdScaled_latest_mSec;   //!
   TBranch        *b_Rec_AdScaled_latest_mNanoSec;   //!
   TBranch        *b_Rec_AdScaled_context_mSite;   //!
   TBranch        *b_Rec_AdScaled_context_mSimFlag;   //!
   TBranch        *b_Rec_AdScaled_context_mTimeStamp_mSec;   //!
   TBranch        *b_Rec_AdScaled_context_mTimeStamp_mNanoSec;   //!
   TBranch        *b_Rec_AdScaled_context_mDetId;   //!
   TBranch        *b_Rec_AdScaled_execNumber;   //!
   TBranch        *b_Rec_AdScaled_jobId_m_data;   //!
   TBranch        *b_Rec_AdScaled_randomState;   //!
   TBranch        *b_Rec_AdScaled_inputHeaders_;   //!
   TBranch        *b_inputHeaders_m_entry;   //!
   TBranch        *b_inputHeaders_m_path;   //!
   TBranch        *b_Rec_AdScaled_site;   //!
   TBranch        *b_Rec_AdScaled_detector;   //!
   TBranch        *b_Rec_AdScaled_triggerType;   //!
   TBranch        *b_Rec_AdScaled_triggerNumber;   //!
   TBranch        *b_Rec_AdScaled_triggerTimeSec;   //!
   TBranch        *b_Rec_AdScaled_triggerTimeNanoSec;   //!
   TBranch        *b_Rec_AdScaled_energy;   //!
   TBranch        *b_Rec_AdScaled_rawEvis;   //!
   TBranch        *b_Rec_AdScaled_enrec;   //!
   TBranch        *b_Rec_AdScaled_eprec;   //!
   TBranch        *b_Rec_AdScaled_x;   //!
   TBranch        *b_Rec_AdScaled_y;   //!
   TBranch        *b_Rec_AdScaled_z;   //!
   TBranch        *b_Rec_AdScaled_t;   //!
   TBranch        *b_Rec_AdScaled_dx;   //!
   TBranch        *b_Rec_AdScaled_dy;   //!
   TBranch        *b_Rec_AdScaled_dz;   //!
   TBranch        *b_Rec_AdScaled_dt;   //!
   TBranch        *b_Rec_AdScaled_energyStatus;   //!
   TBranch        *b_Rec_AdScaled_positionStatus;   //!
   TBranch        *b_Rec_AdScaled_directionStatus;   //!
   TBranch        *b_Rec_AdScaled_energyQuality;   //!
   TBranch        *b_Rec_AdScaled_positionQuality;   //!
   TBranch        *b_Rec_AdScaled_directionQuality;   //!
   TBranch        *b_Rec_AdScaled_errorMatrixDim;   //!
   TBranch        *b_Rec_AdScaled_errorMatrix;   //!

   AdScaled(TTree * /*tree*/ =0) { }
   virtual ~AdScaled() { }
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

   ClassDef(AdScaled,0);
};

#endif

#ifdef AdScaled_cxx
void AdScaled::Init(TTree *tree)
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

   fChain->SetBranchAddress("clID", &clID, &b_Rec_AdScaled_clID);
   fChain->SetBranchAddress("earliest.mSec", &earliest_mSec, &b_Rec_AdScaled_earliest_mSec);
   fChain->SetBranchAddress("earliest.mNanoSec", &earliest_mNanoSec, &b_Rec_AdScaled_earliest_mNanoSec);
   fChain->SetBranchAddress("latest.mSec", &latest_mSec, &b_Rec_AdScaled_latest_mSec);
   fChain->SetBranchAddress("latest.mNanoSec", &latest_mNanoSec, &b_Rec_AdScaled_latest_mNanoSec);
   fChain->SetBranchAddress("context.mSite", &context_mSite, &b_Rec_AdScaled_context_mSite);
   fChain->SetBranchAddress("context.mSimFlag", &context_mSimFlag, &b_Rec_AdScaled_context_mSimFlag);
   fChain->SetBranchAddress("context.mTimeStamp.mSec", &context_mTimeStamp_mSec, &b_Rec_AdScaled_context_mTimeStamp_mSec);
   fChain->SetBranchAddress("context.mTimeStamp.mNanoSec", &context_mTimeStamp_mNanoSec, &b_Rec_AdScaled_context_mTimeStamp_mNanoSec);
   fChain->SetBranchAddress("context.mDetId", &context_mDetId, &b_Rec_AdScaled_context_mDetId);
   fChain->SetBranchAddress("execNumber", &execNumber, &b_Rec_AdScaled_execNumber);
   fChain->SetBranchAddress("jobId.m_data[4]", jobId_m_data, &b_Rec_AdScaled_jobId_m_data);
   fChain->SetBranchAddress("randomState", &randomState, &b_Rec_AdScaled_randomState);
   fChain->SetBranchAddress("inputHeaders", &inputHeaders_, &b_Rec_AdScaled_inputHeaders_);
   fChain->SetBranchAddress("inputHeaders.m_entry", inputHeaders_m_entry, &b_inputHeaders_m_entry);
   fChain->SetBranchAddress("inputHeaders.m_path", inputHeaders_m_path, &b_inputHeaders_m_path);
   fChain->SetBranchAddress("site", &site, &b_Rec_AdScaled_site);
   fChain->SetBranchAddress("detector", &detector, &b_Rec_AdScaled_detector);
   fChain->SetBranchAddress("triggerType", &triggerType, &b_Rec_AdScaled_triggerType);
   fChain->SetBranchAddress("triggerNumber", &triggerNumber, &b_Rec_AdScaled_triggerNumber);
   fChain->SetBranchAddress("triggerTimeSec", &triggerTimeSec, &b_Rec_AdScaled_triggerTimeSec);
   fChain->SetBranchAddress("triggerTimeNanoSec", &triggerTimeNanoSec, &b_Rec_AdScaled_triggerTimeNanoSec);
   fChain->SetBranchAddress("energy", &energy, &b_Rec_AdScaled_energy);
   fChain->SetBranchAddress("rawEvis", &rawEvis, &b_Rec_AdScaled_rawEvis);
   fChain->SetBranchAddress("enrec", &enrec, &b_Rec_AdScaled_enrec);
   fChain->SetBranchAddress("eprec", &eprec, &b_Rec_AdScaled_eprec);
   fChain->SetBranchAddress("x", &x, &b_Rec_AdScaled_x);
   fChain->SetBranchAddress("y", &y, &b_Rec_AdScaled_y);
   fChain->SetBranchAddress("z", &z, &b_Rec_AdScaled_z);
   fChain->SetBranchAddress("t", &t, &b_Rec_AdScaled_t);
   fChain->SetBranchAddress("dx", &dx, &b_Rec_AdScaled_dx);
   fChain->SetBranchAddress("dy", &dy, &b_Rec_AdScaled_dy);
   fChain->SetBranchAddress("dz", &dz, &b_Rec_AdScaled_dz);
   fChain->SetBranchAddress("dt", &dt, &b_Rec_AdScaled_dt);
   fChain->SetBranchAddress("energyStatus", &energyStatus, &b_Rec_AdScaled_energyStatus);
   fChain->SetBranchAddress("positionStatus", &positionStatus, &b_Rec_AdScaled_positionStatus);
   fChain->SetBranchAddress("directionStatus", &directionStatus, &b_Rec_AdScaled_directionStatus);
   fChain->SetBranchAddress("energyQuality", &energyQuality, &b_Rec_AdScaled_energyQuality);
   fChain->SetBranchAddress("positionQuality", &positionQuality, &b_Rec_AdScaled_positionQuality);
   fChain->SetBranchAddress("directionQuality", &directionQuality, &b_Rec_AdScaled_directionQuality);
   fChain->SetBranchAddress("errorMatrixDim", &errorMatrixDim, &b_Rec_AdScaled_errorMatrixDim);
   fChain->SetBranchAddress("errorMatrix", &errorMatrix, &b_Rec_AdScaled_errorMatrix);
}

Bool_t AdScaled::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef AdScaled_cxx
