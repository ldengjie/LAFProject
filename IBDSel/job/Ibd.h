//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Oct 13 03:38:07 2012 by ROOT version 5.26/00e
// from TTree IbdTree/IbdTree
// found on file: 22949_IBD.root
//////////////////////////////////////////////////////////

#ifndef Ibd_h
#define Ibd_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TSelector.h>
#include <iostream> 
#include <math.h>
#include  <string>
using namespace std;
class Ibd : public TSelector {
    public :
        double livetime[3][4];
        double livetime0[3][4];
        double t_result[49][8];
        double epsi_mu[3][4];
        double epsi_mu0[3][4];
        double daqtime[3][4];
        double daqtime0[3][4];
        double epsi_multi[3][4];
        double epsi_multi0[3][4];
        int site;
        string dataVer;
        //static const  int nBins=200;
        TTree          *fChain;   //!pointer to the lyzed TTree or TChain
        TFile* file;
        double UpperNum[4];
        double LowerNum[4];
        // Declaration of leaf types
        Int_t           isIbd;
        Int_t           isLi;
        Int_t           det;
        Float_t         energy[2];
        Float_t         x[2];
        Float_t         y[2];
        Float_t         z[2];
        Int_t           fileNum[2];
        Long64_t        entry[2];
        Int_t        trigNum[2];
        Double_t         timeInterval;
        Int_t           delayedTrigTimeSec;
        Double_t         promptT2Muon[22];
        Double_t         delayedT2Muon[22];
        int Num0000ldj[4];//if delete or comment this line,Numo[4] will be abnormal , Numo[4] become a very very large value 1259902592;if swap this line with next line,abnormal like the former.it's strange,don't know the reason.
        int Num000ldj[4];//if delete or comment this line,Numo[4] will be abnormal , Numo[4] become a very very large value 1259902592;if swap this line with next line,abnormal like the former.it's strange,don't know the reason.
        int Numo[4];
        int num00;
        int Num1[4];
        int Num2[4];
        int tNumo;
        int tNum1;
        int tNum2;
        int ADNum;
        TString histname; 
        TH1F *t2lastmuonWithrpc[4];
        TH1F *t2lastmuonWithoutrpc[4];
        TH1F *FnProEWithrpc[4];
        TH1F *FnProEWithoutrpc[4];
        TH1F *tFnProEWithrpc;
        TH1F *tFnProEWithoutrpc;
        TH2F *pxy[4];
        TH2F *prz[4];
        TH1F *pE[4];
        TH2F *dxy[4];
        TH2F *drz[4];
        TH1F *dE[4];
        TH1F *intervalT[4];
        TH2F *pxy0[4];
        TH2F *prz0[4];
        TH1F *pE0[4];
        TH2F *dxy0[4];
        TH2F *drz0[4];
        TH1F *dE0[4];
        TH1F *intervalT0[4];
        double* BinWt;
        bool dofit4fn;
        TH1F* h4fnRate;
        TH1F* h4fnFit[7];
        double binLowEdge[8];
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

        Ibd(TTree * /*tree*/ =0) { }
        virtual ~Ibd() { }
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
        virtual void    Terminate0();
        virtual void    FillIbd(int itype);

        ClassDef(Ibd,0);
};

#endif

#ifdef Ibd_cxx
void Ibd::Init(TTree *tree)
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

Bool_t Ibd::Notify()
{
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return kTRUE;
}

#endif // #ifdef Ibd_cxx
