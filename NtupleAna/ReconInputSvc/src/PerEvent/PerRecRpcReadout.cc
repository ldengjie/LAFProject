#include "PerRecRpcReadout.h"
#include <iostream>
#include <cstdlib>

PerRecRpcReadout::PerRecRpcReadout(TTree* tree)
    : fChain(tree)
{
    fChain->SetMakeClass(1);

    UInt_t nBr = 0;
    fChain->SetBranchStatus("*", 0, &nBr);

    fChain->SetBranchAddress("combinationType",  &combinationType);
    fChain->SetBranchAddress("eventType",        &eventType);
    fChain->SetBranchAddress("nClusters",        &nClusters);
    fChain->SetBranchAddress("nTracks",          &nTracks);

    fChain->SetBranchAddress("clusters",         &clusters_);
    fChain->SetBranchAddress("clusters.status",  clusters_status);
    fChain->SetBranchAddress("clusters.type",    clusters_type);
    fChain->SetBranchAddress("clusters.maxHitsInLayer", clusters_maxHitsInLayer);
    fChain->SetBranchAddress("clusters.is4of4",  clusters_is4of4);
    fChain->SetBranchAddress("clusters.x",       clusters_x);
    fChain->SetBranchAddress("clusters.y",       clusters_y);
    fChain->SetBranchAddress("clusters.z",       clusters_z);
    fChain->SetBranchAddress("clusters.t",       clusters_t);
    fChain->SetBranchAddress("clusters.quality", clusters_quality);

    fChain->SetBranchAddress("tracks",           &tracks_);
    fChain->SetBranchAddress("tracks.start",     tracks_start);
    fChain->SetBranchAddress("tracks.end",       tracks_end);
}

PerRecRpcReadout::~PerRecRpcReadout()
{
}

DataSec* PerRecRpcReadout::ToTran(long entry, int iFile, long iEvent)
{
    DataSec* obj = 0;
    if ( fChain->GetEntry(entry) > 0 ) {
	if (fChain->GetTreeNumber() != iFile ) exit(1);
	RecRpcReadout* _obj = new RecRpcReadout();
	_obj->combinationType = combinationType;
	_obj->eventType = eventType;
	_obj->nClusters = nClusters;
	_obj->nTracks   = nTracks;

	for ( int i = 0; i < nClusters; ++i ) {
	    RecRpcReadout::Cluster* cluster = new RecRpcReadout::Cluster(
		    clusters_status[i],
		    clusters_type[i],
		    clusters_maxHitsInLayer[i],
		    clusters_is4of4[i],
		    clusters_x[i],
		    clusters_y[i],
		    clusters_z[i],
		    clusters_t[i],
		    clusters_quality[i]
		    );
	    _obj->clusters.push_back(cluster);
	}

	for ( int i = 0; i < nTracks; ++i ) {
	    //std::cout << tracks_start[i] << "  to  " << tracks_end[i] << std::endl;
	    RecRpcReadout::Cluster* start = _obj->clusters[ tracks_start[i] ];
	    RecRpcReadout::Cluster* end   = _obj->clusters[ tracks_end[i] ];
	    RecRpcReadout::Track* track = new RecRpcReadout::Track(start, end);
	    _obj->tracks.push_back(track);
	}

	obj = _obj;
    }
    return obj;
}
