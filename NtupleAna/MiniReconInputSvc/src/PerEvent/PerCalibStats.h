#ifndef PER_CALIB_STATS_H
#define PER_CALIB_STATS_H

#include "LafKernel/PhyEvent/CalibStats.h"
#include "IPerDataSec.h"
#include "TChain.h"

class PerCalibStats : protected CalibStats, public IPerDataSec
{
    public :

	PerCalibStats(TTree* tree);
	virtual ~PerCalibStats();

	virtual DataSec* ToTran(long entry);

    protected :

	TTree          *fChain;
};

#endif
