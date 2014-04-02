#include  <iostream>
#include  "TTree.h"
//#include  "MuonVeto/MuonVeto.h" 
#include  "LafKernel/PhyEvent.h"
#include  "LafKernel/PhyEventBuf.h"
#include  <vector>
#include  "LafKernel/OptionParser.h"
#include  "LafKernel/AlgBase.h"
#include  "LafKernel/DataBuffer.h"
#include  "LafKernel/GlobalVar.h"
#include  "LafKernel/LafLog.h"
#include  "LiveTimeSvc/LiveTimeSvc.h"
#include "LafKernel/PhyEvent/CalibReadout.h"
#include "LafKernel/PhyEvent/RecRpcReadout.h"

using namespace std;  

class SimplifiedRoot : public AlgBase
{
  public:
	SimplifiedRoot(const std::string& name);
	virtual ~SimplifiedRoot(){}

	virtual bool initialize();
	virtual bool execute();
	virtual bool finalize();
  private:

	PhyEventBuf* EvtBuffer;
	PhyEvent* CurEvent;
	CalibReadout* calib;
	RecRpcReadout* recrpc;

	TTree* AdScaledTree;
	int site;
	int detector;
	int triggerType;
	int triggerNumber;
	int triggerTimeSec;
	int triggerTimeNanoSec;
	float energy;
	float rawEvis;
	float enrec;
	float eprec;
	float x;
	float y;
	float z;

	TTree* CalibStatsTree;
	int nHit;
	float MaxQ;
	float Quadrant;
	float MaxQ_2inchPMT;
	float Column_2inchPMT;
	float MiddleTimeRMS;
	float tRMS;
	float nPEMax;
	float nPEMedian;
	float nPERMS;
	float nPESum;

	TTree* CalibReadoutHeaderTree; 
    unsigned int               nHitsRpc;
    std::vector<unsigned int>  rpcRow;
    std::vector<unsigned int>  rpcColumn;
    std::vector<unsigned int>  rpcLayer;

	/*
	TTree* RpcSimpleTree;
	unsigned int           eventType; 
	int nClusters;
	std::vector<class Cluster*>  cluster;
	//class Cluster  cluster;
	*/
};
