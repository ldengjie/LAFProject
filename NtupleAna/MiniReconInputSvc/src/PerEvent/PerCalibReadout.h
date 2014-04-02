#ifndef PER_CALIB_READOUT_H
#define PER_CALIB_READOUT_H

#include "LafKernel/PhyEvent/CalibReadout.h"
#include "IPerDataSec.h"
#include "TChain.h"

class PerCalibReadout : protected CalibReadout, public IPerDataSec
{
    public :

	PerCalibReadout(TTree* tree);
	virtual ~PerCalibReadout();

	virtual DataSec* ToTran(long entry);

    protected :

	TTree          *fChain;
};

#endif
