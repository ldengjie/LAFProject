#include "PerCalibStats.h"
#include <cstdlib>

PerCalibStats::PerCalibStats(TTree* tree)
    : fChain(tree)
{
    fChain->SetMakeClass(1);

    // skip the unnecessary branches
    UInt_t nBr = 0;
    fChain->SetBranchStatus("*", 0, &nBr);

    fChain->SetBranchAddress("ColumnKurtosis",     &ColumnKurtosis);
    fChain->SetBranchAddress("EarlyCharge",        &EarlyCharge);
    fChain->SetBranchAddress("Kurtosis",           &Kurtosis);
    fChain->SetBranchAddress("LateCharge",         &LateCharge);
    fChain->SetBranchAddress("MainPeakRMS",        &MainPeakRMS);
    //fChain->SetBranchAddress("MiddleTimeRMS",      &MiddleTimeRMS);
    fChain->SetBranchAddress("NominalCharge",      &NominalCharge);
    fChain->SetBranchAddress("PeakRMS",            &PeakRMS);
    fChain->SetBranchAddress("Q1",                 &Q1);
    fChain->SetBranchAddress("Q2",                 &Q2);
    fChain->SetBranchAddress("Q3",                 &Q3);
    fChain->SetBranchAddress("QuadrantQ1",         &QuadrantQ1);
    fChain->SetBranchAddress("QuadrantQ2",         &QuadrantQ2);
    fChain->SetBranchAddress("QuadrantQ3",         &QuadrantQ3);
    fChain->SetBranchAddress("QuadrantQ4",         &QuadrantQ4);
    fChain->SetBranchAddress("RingKurtosis",       &RingKurtosis);
    fChain->SetBranchAddress("SecondPeakRMS",      &SecondPeakRMS);
    fChain->SetBranchAddress("charge_sum_flasher_max", &charge_sum_flasher_max);
    fChain->SetBranchAddress("flasher_column",     &flasher_column);
    fChain->SetBranchAddress("flasher_flag",       &flasher_flag);
    fChain->SetBranchAddress("flasher_ring",       &flasher_ring);
    fChain->SetBranchAddress("tRMS",               &tRMS);
    fChain->SetBranchAddress("time_PSD",           &time_PSD);
    fChain->SetBranchAddress("time_PSD1",          &time_PSD1);
    fChain->SetBranchAddress("time_PSD_local_RMS", &time_PSD_local_RMS);
}

PerCalibStats::~PerCalibStats()
{
}

DataSec* PerCalibStats::ToTran(long entry, int iFile, long iEvent)
{
    DataSec* obj = 0;
    if ( fChain->GetEntry(entry) > 0 ) {
	if (fChain->GetTreeNumber() != iFile ) exit(1);
	obj = new CalibStats(*this);
    }
    return obj;
}
