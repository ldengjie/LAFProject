#include "PerMuonRecSimple.h"
#include "TROOT.h"

PerMuonRecSimple::PerMuonRecSimple(TTree* tree)
    : fChain(tree),
      imap(0)
{
}

PerMuonRecSimple::~PerMuonRecSimple()
{
    delete [] imap;
}

DataSec* PerMuonRecSimple::ToTran(long entry, int iFile, long iEvent)
{
    if ( imap == 0 ) init();

    DataSec* obj = 0;
    std::map<long, long>::iterator it = imap[iFile].find(iEvent);
    if ( it != imap[iFile].end() && fChain->GetEntry(it->second) > 0 ) {
	obj = new MuonRecSimple(*this);
    }
    return obj;
}

void PerMuonRecSimple::init()
{
    TChain* ftmp = dynamic_cast<TChain*>(fChain);
    imap = new std::map<long, long>[ftmp->GetNtrees()];

    fChain->SetMakeClass(1);
    // skip the unnecessary branches
    UInt_t nBr = 0;
    fChain->SetBranchStatus("*", 0, &nBr);

    // initialize the execNumber map
    int en;
    fChain->SetBranchAddress("execNumber", &en);

    long nn = fChain->GetEntries();
    for ( long i = 0; i < nn; ++i ) {
	fChain->GetEntry(i);
	imap[fChain->GetTreeNumber()][en] = i;
    }

    // initialize the class vars
    fChain->SetBranchStatus("*", 0, &nBr);
    fChain->SetBranchAddress("DetectorId", &DetectorId);
    fChain->SetBranchAddress("EndIndex",   &EndIndex);
    fChain->SetBranchAddress("FiredDet",   &FiredDet);
    fChain->SetBranchAddress("Flag",       &Flag);
    fChain->SetBranchAddress("IWSNhit",    &IWSNhit);
    fChain->SetBranchAddress("OWSNhit",    &OWSNhit);
    fChain->SetBranchAddress("PointX",     &PointX);
    fChain->SetBranchAddress("PointY",     &PointY);
    fChain->SetBranchAddress("PointZ",     &PointZ);
    fChain->SetBranchAddress("StartIndex", &StartIndex);
}
