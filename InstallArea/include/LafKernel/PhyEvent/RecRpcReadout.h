#ifndef REC_RPC_READOUT_h
#define REC_RPC_READOUT_h

#include "TVector3.h"
#include "LafKernel/PhyEvent/DataSec.h"

class RecRpcReadout : public DataSec {

    public :

	virtual ~RecRpcReadout();

	class Cluster {
	    public :
		int status;
		int type;
		int maxHitsInLayer;
		bool  is4of4;
		float x;
		float y;
		float z;
		float t;
		float quality;

		Cluster(int _status, int _type, int _maxHitsInLayer, bool _is4of4,
			float _x, float _y, float _z, float _t, float _quality);
		const TVector3 position();
	};

	class Track {
	    public :
		Cluster* start;
		Cluster* end;

		Track(Cluster* _start, Cluster* _end);
		const TVector3 direction();
	};

	int  maxHitsInLayer();
	bool is4of4();

	unsigned int           combinationType;
	unsigned int           eventType;
	int                    nClusters;
	int                    nTracks;
	std::vector<Cluster*>  clusters;
	std::vector<Track*>    tracks;
};

#endif
