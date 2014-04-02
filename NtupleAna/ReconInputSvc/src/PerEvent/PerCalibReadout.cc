#include "PerCalibReadout.h"
#include "TROOT.h"
#include <cstdlib>

PerCalibReadout::PerCalibReadout(TTree* tree)
    : fChain(tree)
{
    gROOT->ProcessLine("#include <vector>");

    fChain->SetMakeClass(1);

    // skip the unnecessary branches
    UInt_t nBr = 0;
    fChain->SetBranchStatus("*", 0, &nBr);

    fChain->SetBranchAddress("nHitsAD",          &nHitsAD);
    fChain->SetBranchAddress("timeAD",           &timeAD);
    fChain->SetBranchAddress("chargeAD",         &chargeAD);
    fChain->SetBranchAddress("hitCountAD",       &hitCountAD);
    fChain->SetBranchAddress("ring",             &ring);
    fChain->SetBranchAddress("column",           &column);
    fChain->SetBranchAddress("nHitsAD_calib",    &nHitsAD_calib);
    fChain->SetBranchAddress("timeAD_calib",     &timeAD_calib);
    fChain->SetBranchAddress("chargeAD_calib",   &chargeAD_calib);
    fChain->SetBranchAddress("hitCountAD_calib", &hitCountAD_calib);
    fChain->SetBranchAddress("topOrBottom",      &topOrBottom);
    fChain->SetBranchAddress("acuColumn",        &acuColumn);
    fChain->SetBranchAddress("nHitsPool",        &nHitsPool);
    fChain->SetBranchAddress("timePool",         &timePool);
    fChain->SetBranchAddress("chargePool",       &chargePool);
    fChain->SetBranchAddress("hitCountPool",     &hitCountPool);
    fChain->SetBranchAddress("wallNumber",       &wallNumber);
    fChain->SetBranchAddress("wallSpot",         &wallSpot);
    fChain->SetBranchAddress("inwardFacing",     &inwardFacing);
    fChain->SetBranchAddress("nHitsRpc",         &nHitsRpc);
    fChain->SetBranchAddress("rpcRow",           &rpcRow);
    fChain->SetBranchAddress("rpcColumn",        &rpcColumn);
    fChain->SetBranchAddress("rpcLayer",         &rpcLayer);
    fChain->SetBranchAddress("rpcStrip",         &rpcStrip);
    fChain->SetBranchAddress("rpcFromRot",       &rpcFromRot);
}

PerCalibReadout::~PerCalibReadout()
{
}

DataSec* PerCalibReadout::ToTran(long entry, int iFile, long iEvent)
{
    DataSec* obj = 0;
    if ( fChain->GetEntry(entry) > 0 ) {
	if (fChain->GetTreeNumber() != iFile ) exit(1);
	obj = new CalibReadout(*this);
    }
    return obj;
}
