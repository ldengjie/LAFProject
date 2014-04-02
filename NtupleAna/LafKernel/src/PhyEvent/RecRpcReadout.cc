#include "LafKernel/PhyEvent/RecRpcReadout.h"

RecRpcReadout::~RecRpcReadout()
{
    for ( int i = 0; i < nClusters; ++i ) {
	delete clusters[i];
    }
    for ( int i = 0; i < nTracks; ++i ) {
	delete tracks[i];
    }
}

RecRpcReadout::Cluster::Cluster(int _status, int _type, int _maxHitsInLayer, bool _is4of4,
	float _x, float _y, float _z, float _t, float _quality)
    : status(_status),
      type(_type),
      maxHitsInLayer(_maxHitsInLayer),
      is4of4(_is4of4),
      x(_x),
      y(_y),
      z(_z),
      t(_t),
      quality(_quality)
{
}

const TVector3 RecRpcReadout::Cluster::position()
{
    return TVector3(x, y, z);
}

RecRpcReadout::Track::Track(RecRpcReadout::Cluster* _start, RecRpcReadout::Cluster* _end)
    : start(_start),
      end(_end)
{
}

const TVector3 RecRpcReadout::Track::direction()
{
    TVector3 vDiff = end->position() - start->position();
    vDiff.SetMag(1.0);
    return vDiff;
}

int  RecRpcReadout::maxHitsInLayer()
{
    int res = 0;
    std::vector<Cluster*>::iterator end = clusters.end();
    for ( std::vector<Cluster*>::iterator it = clusters.begin(); it != end; ++it ) {
	int _max = (*it)->maxHitsInLayer;
	if ( _max > res ) {
	    res = _max;
	}
    }
    return res;
}

bool RecRpcReadout::is4of4()
{
    std::vector<Cluster*>::iterator end = clusters.end();
    for ( std::vector<Cluster*>::iterator it = clusters.begin(); it != end; ++it ) {
	if ( (*it)->is4of4 ) return true;
    }
    return false;
}
