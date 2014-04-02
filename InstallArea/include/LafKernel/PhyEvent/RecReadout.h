#ifndef REC_READOUT_H
#define REC_READOUT_H

#include "LafKernel/PhyEvent/DataSec.h"

class RecReadout : public DataSec {

    public :

	// Declaration of leaf types
	float         energy;
	float         rawEvis;
	float         x;
	float         y;
	float         z;
};

#endif
