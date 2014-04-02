#ifndef PER_REC_RPC_READOUT_H
#define PER_REC_RPC_READOUT_H

#include "LafKernel/PhyEvent/RecRpcReadout.h"
#include "IPerDataSec.h"
#include "TChain.h"

class PerRecRpcReadout : public IPerDataSec
{
    public :

	PerRecRpcReadout(TTree* tree);
	virtual ~PerRecRpcReadout();

	virtual DataSec* ToTran(long entry, int iFile, long iEvent);

    protected :

	TTree          *fChain;

	unsigned int combinationType;
	unsigned int eventType;
	int          nClusters;
	int          nTracks;

	int          clusters_;
	int          clusters_status[100];
	int          clusters_type[100];
	int          clusters_maxHitsInLayer[100];
	bool         clusters_is4of4[100];
	float        clusters_x[100];
	float        clusters_y[100];
	float        clusters_z[100];
	float        clusters_t[100];
	float        clusters_quality[100];

	int          tracks_;
	int          tracks_start[100];
	int          tracks_end[100];
};

#endif
