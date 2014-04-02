#ifndef CALIB_READOUT_H
#define CALIB_READOUT_H

#include "LafKernel/PhyEvent/DataSec.h"
#include <vector>

class CalibReadout : public DataSec {

    public :

	// Declaration of leaf types
	unsigned int               nHitsAD;
	std::vector<float>         timeAD;
	std::vector<float>         chargeAD;
	std::vector<unsigned int>  hitCountAD;
	std::vector<unsigned int>  ring;
	std::vector<unsigned int>  column;
	unsigned int               nHitsAD_calib;
	std::vector<float>         timeAD_calib;
	std::vector<float>         chargeAD_calib;
	std::vector<unsigned int>  hitCountAD_calib;
	std::vector<unsigned int>  topOrBottom;
	std::vector<unsigned int>  acuColumn;
	unsigned int               nHitsPool;
	std::vector<float>         timePool;
	std::vector<float>         chargePool;
	std::vector<unsigned int>  hitCountPool;
	std::vector<unsigned int>  wallNumber;
	std::vector<unsigned int>  wallSpot;
	std::vector<unsigned int>  inwardFacing;
	unsigned int               nHitsRpc;
	std::vector<unsigned int>  rpcRow;
	std::vector<unsigned int>  rpcColumn;
	std::vector<unsigned int>  rpcLayer;
	std::vector<unsigned int>  rpcStrip;
	std::vector<bool>          rpcFromRot;
};

#endif
