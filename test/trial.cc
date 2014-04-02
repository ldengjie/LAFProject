#include <vector>
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TInterpreter.h"

void write_tree(const char *file_name = "trial.root")
{
    if (!file_name || !(*file_name)) return; // just a precaution

#if 1 /* 0 or 1 */

    TFile *f = new TFile(file_name, "RECREATE");
    TTree *tree = new TTree("tree", "tree");
    std::vector<std::vector<float> > _parFloatVal;
    tree->Branch("fltvv", &_parFloatVal);

    for (int i = 0; i < 10; i++) {
        _parFloatVal.clear();

        for (int j = 0; j < 10; j++) {
            std::vector<float> floatVec;
            floatVec.push_back(j); // just an example
            floatVec.push_back(9 - j); // just an example
            _parFloatVal.push_back(floatVec);
        } // for (int j ...

        tree->Fill();
    } // for (int i ...

    tree->Write();
    delete f; // automatically deletes "tree", too

#else /* 0 or 1 */

    TFile *f = new TFile(file_name, "RECREATE");
    TTree *tree = new TTree("tree", "tree");
    std::vector<std::vector<float> > *_parFloatPtr = 0;
    tree->Branch("fltvv", &_parFloatPtr);

    for (int i = 0; i < 10; i++) {
        if (_parFloatPtr) _parFloatPtr->clear();
        else _parFloatPtr = new std::vector<std::vector<float> >;

        for (int j = 0; j < 10; j++) {
            std::vector<float> floatVec;
            floatVec.push_back(j); // just an example
            floatVec.push_back(9 - j); // just an example
            _parFloatPtr->push_back(floatVec);
        } // for (int j ...

        tree->Fill();
    } // for (int i ...

    tree->Write();
    delete f; // automatically deletes "tree", too
    delete _parFloatPtr;

#endif /* 0 or 1 */

} // void write_tree ...

void read_tree(const char *file_name = "trial.root")
{
    if (!file_name || !(*file_name)) return; // just a precaution
    TFile *f = new TFile(file_name, "READ");
    TTree *chain;
    f->GetObject("tree", chain);

    if (chain) {
        chain->Print();
        chain->Draw("fltvv");

        //
        // This is a "TChain-ready" loop skeleton where:
        //   "entry" is the global entry number in the "chain"
        //   "local_entry" is the entry number in the current TTree
        // Note that the argument to GetEntry must be:
        //   "entry" for TChain::GetEntry
        //   "local_entry" for TTree::GetEntry and TBranch::GetEntry
        //
        // To read only selected branches:
        //   METHOD1: insert statements like
        //     chain->SetBranchStatus("*", 0); // disable all branches
        //     chain->SetBranchStatus("branchname", 1); // activate "branchname"
        //   METHOD2: replace line
        //     chain->GetEntry(entry); // read all ("activated") branches
        //   by (note: you need to create/get the "b_branchname" pointer first)
        //     b_branchname->GetEntry(local_entry); // read only this branch
        //

        std::vector<std::vector<float> > *_parFloatPtr = 0;
        chain->SetBranchAddress("fltvv", &_parFloatPtr);

        for(Long64_t entry = 0; entry < chain->GetEntriesFast(); entry++) {
            Long64_t local_entry = chain->LoadTree(entry);
            if (local_entry < 0) break;

            chain->GetEntry(entry);

            if (!_parFloatPtr) break; // just a precaution

            std::cout << "Entry " << entry << " : ";
            for (unsigned int i = 0; i < _parFloatPtr->size(); i++) {
                for (unsigned int j = 0; j < (_parFloatPtr->at(i)).size(); j++) {
                    std::cout << (*_parFloatPtr)[i][j] << " ";
                } // for (unsigned int j ...
                std::cout << ": ";
            } // for (unsigned int i ...
            std::cout << std::endl;

        } // for(Long64_t entry ...

        chain->ResetBranchAddresses();
        delete _parFloatPtr;
    } // if (chain) ...

    delete f; // automatically deletes "chain", too
} // void read_tree ...

// "ROOT Script" entry point (the same name as the "filename's base").
void trial(const char *file_name = "trial.root")
{
    if (!file_name || !(*file_name)) return; // just a precaution

#if !defined(__CINT__)
    if (!(gInterpreter->IsLoaded("vector")))
        gInterpreter->ProcessLine("#include <vector>");
    gSystem->Exec("rm -f AutoDict*vector*vector*float*");
    gInterpreter->GenerateDictionary("vector<vector<float> >", "vector");
    // gInterpreter->GenerateDictionary("std::vector<std::vector<float> >", "vector");
#endif /* !defined(__CINT__) */

    write_tree(file_name);
    read_tree(file_name);
}

#if !defined(__CINT__) && !defined(__ACLIC__)
// "Standalone Application" entry point ("main").
int main(int /*argc*/, char ** /*argv*/)
{
    trial("trial.root"); // just call the "ROOT Script"
    return 0;
}
#endif /* !defined(__CINT__) && !defined(__ACLIC__) */
