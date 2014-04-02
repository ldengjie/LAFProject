#ifndef PER_MUON_REC_SIMPLE_H
#define PER_MUON_REC_SIMPLE_H

#include "LafKernel/PhyEvent/MuonRecSimple.h"
#include "IPerDataSec.h"
#include "TChain.h"

class PerMuonRecSimple : protected MuonRecSimple, public IPerDataSec
{
    public :

	PerMuonRecSimple(TTree* tree);
	virtual ~PerMuonRecSimple();

	virtual DataSec* ToTran(long entry, int iFile, long iEvent);

    protected :

	void   init();

	TTree          *fChain;

	std::map<long, long>*  imap;
};

#endif
