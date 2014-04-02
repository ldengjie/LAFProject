#ifndef MuonRecSimple_h
#define MuonRecSimple_h

#include "LafKernel/PhyEvent/DataSec.h"
#include <vector>

class MuonRecSimple : public DataSec {

    public :

	// Declaration of leaf types
	std::vector<int>     *DetectorId;
	int             EndIndex;
	int             FiredDet;
	int             Flag;
	int             IWSNhit;
	int             OWSNhit;
	std::vector<float>   *PointX;
	std::vector<float>   *PointY;
	std::vector<float>   *PointZ;
	int             StartIndex;


	MuonRecSimple();
	MuonRecSimple(const MuonRecSimple& other);
	virtual ~MuonRecSimple();
};

#endif
