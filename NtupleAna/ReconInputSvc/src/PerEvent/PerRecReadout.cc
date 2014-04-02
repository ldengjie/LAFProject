#include "PerRecReadout.h"
#include <iostream>
#include <cstdlib>

PerRecReadout::PerRecReadout(TTree* tree)
    : fChain(tree)
{
    fChain->SetMakeClass(1);

    // skip the unnecessary branches
    UInt_t nBr = 0;
    fChain->SetBranchStatus("*", 0, &nBr);

    fChain->SetBranchAddress("energy", &energy);
    fChain->SetBranchAddress("rawEvis", &rawEvis);
    fChain->SetBranchAddress("x", &x);
    fChain->SetBranchAddress("y", &y);
    fChain->SetBranchAddress("z", &z);
}

PerRecReadout::~PerRecReadout()
{
}

DataSec* PerRecReadout::ToTran(long entry, int iFile, long iEvent)
{
    DataSec* obj = 0;
    if ( fChain->GetEntry(entry) > 0 ) {
	if (fChain->GetTreeNumber() != iFile ) exit(1);
	obj = new RecReadout(*this);
    }
    return obj;
}
