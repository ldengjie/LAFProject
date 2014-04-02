#ifndef PER_REC_READOUT_H
#define PER_REC_READOUT_H

#include "LafKernel/PhyEvent/RecReadout.h"
#include "IPerDataSec.h"
#include "TChain.h"

class PerRecReadout : protected RecReadout, public IPerDataSec
{
    public :

	PerRecReadout(TTree* tree);
	virtual ~PerRecReadout();

	virtual DataSec* ToTran(long entry, int iFile, long iEvent);

    protected :

	TTree          *fChain;
};

#endif
