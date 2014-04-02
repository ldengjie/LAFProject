#include "SimplifiedRoot.h"
#include "LafKernel/AlgFactory.h"

DECLARE_ALGORITHM(SimplifiedRoot);

SimplifiedRoot::SimplifiedRoot(const std::string& name)
: AlgBase(name)
{
}

bool SimplifiedRoot::initialize()
{
	EvtBuffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));

	AdScaledTree = ntupleSvc()->bookTree("FILE2/Event/Rec/AdScaled","AdScaled");
    AdScaledTree->Branch("site",&site,"site/I" ); 
    AdScaledTree->Branch("detector",&detector,"detector/I");
    AdScaledTree->Branch("triggerType",&triggerType,"triggerType/I");
    AdScaledTree->Branch("triggerNumber",&triggerNumber,"triggerNumber/I");
    AdScaledTree->Branch("triggerTimeSec",&triggerTimeSec,"triggerTimeSec/I");
    AdScaledTree->Branch("triggerTimeNanoSec",&triggerTimeNanoSec,"triggerTimeNanoSec/I");
    AdScaledTree->Branch("energy",&energy,"energy/F");
    AdScaledTree->Branch("rawEvis",&rawEvis,"rawEvis/F");
    AdScaledTree->Branch("enrec",&enrec,"enrec/F");
    AdScaledTree->Branch("eprec",&eprec,"eprec/F");
    AdScaledTree->Branch("x",&x,"x/F");
    AdScaledTree->Branch("y",&y,"y/F");
    AdScaledTree->Branch("z",&z,"z/F");
	AdScaledTree->SetBasketSize("*",3200000);

	CalibStatsTree = ntupleSvc()->bookTree("FILE2/Event/Data/CalibStats","CalibStats");
    CalibStatsTree->Branch("nHit",&nHit,"nHit/I"); 
    CalibStatsTree->Branch("MaxQ",&MaxQ,"MaxQ/F");
    CalibStatsTree->Branch("Quadrant",&Quadrant,"Quadrant/F");
    CalibStatsTree->Branch("MaxQ_2inchPMT",&MaxQ_2inchPMT,"MaxQ_2inchPMT/F");
    CalibStatsTree->Branch("Column_2inchPMT",&Column_2inchPMT,"Column_2inchPMT/I");
    CalibStatsTree->Branch("MiddleTimeRMS",&MiddleTimeRMS,"MiddleTimeRMS/F");
    CalibStatsTree->Branch("tRMS",&tRMS,"tRMS/F");
    CalibStatsTree->Branch("nPEMax",&nPEMax,"nPEMax/F");
    CalibStatsTree->Branch("nPEMedian",&nPEMedian,"nPEMedian/F");
    CalibStatsTree->Branch("nPERMS",&nPERMS,"nPERMS/F");
    CalibStatsTree->Branch("nPESum",&nPESum,"nPESum/F");
	CalibStatsTree->SetBasketSize("*",3200000);

	CalibReadoutHeaderTree = ntupleSvc()->bookTree("FILE2/Event/CalibReadout/CalibReadoutHeader","CalibReadoutHeader");
	CalibReadoutHeaderTree->Branch("nHitsRpc",&nHitsRpc,"nHitsRpc/i");
	CalibReadoutHeaderTree->Branch("rpcRow",&rpcRow,"rpcRow/i");
	CalibReadoutHeaderTree->Branch("rpcColumn",&rpcColumn,"rpcColumn/i");
	CalibReadoutHeaderTree->Branch("rpcLayer",&rpcLayer,"rpcLayer/i");
/*
	RpcSimpleTree = ntupleSvc()->bookTree("Event/Rec/RpcSimple","RpcSimple");
	RpcSimpleTree->Branch("eventType",&eventType,"eventType/i");
	RpcSimpleTree->Branch("clusters","Cluster",&cluster,32000,99);
*/
	std::cout<<"SimplifiedRoot initializing..."<<endl;
	return true;
}

bool SimplifiedRoot::execute()
{
	
	PhyEvent* CurEvent= EvtBuffer->curEvt();

	if( (CurEvent->m_det==5 || CurEvent->m_det==6)&&(CurEvent->m_nPmt<=10) )
	{
		return true;
	}
/*
	if( CurEvent->isRPC() )
	{
		return true;
	}
*/
	if( CurEvent->isAD() )
	{
		if(!( CurEvent->energy()>0.7) )
		{
			return true;
		}

		float t2preAD=10.e6;
		float t2nextAD=10.e6;
		int  det = CurEvent->m_det;
		DataBuffer<PhyEvent>::Iterator preit=EvtBuffer->find(CurEvent);
		--preit;
		while( preit >= EvtBuffer->begin() )
		{
			PhyEvent* event = (*preit);
			if( event->m_det==det )
			{
				t2preAD=CurEvent->m_trigTime-event->m_trigTime;
				break;
			}
			--preit;
		}
		DataBuffer<PhyEvent>::Iterator nextit=EvtBuffer->find(CurEvent);
		++nextit;
		while( nextit <= EvtBuffer->end() )
		{
			PhyEvent* event = (*nextit);
			if( event->m_det==det )
			{
				t2nextAD=event->m_trigTime-CurEvent->m_trigTime;
				break;
			}
			++nextit;
		}

		if( (t2nextAD>3.e-4) && (t2preAD>3.e-4) )
		{
			return true;
		}

	}

	site=CurEvent->m_site;
	detector=CurEvent->m_det;
	triggerType=CurEvent->m_trigType;
	triggerNumber=CurEvent->m_trigNum;
	energy=CurEvent->m_energy;
	rawEvis=CurEvent->m_rawEvis;
	enrec=CurEvent->m_enrec;
	eprec=CurEvent->m_eprec;
	x=CurEvent->m_x;
	y=CurEvent->m_y;
	z=CurEvent->m_z;
	triggerTimeSec=CurEvent->m_trigTime.GetSec();
	triggerTimeNanoSec=CurEvent->m_trigTime.GetNanoSec();

	nHit=CurEvent->m_nPmt;
	MaxQ=CurEvent->m_maxQ;
	Quadrant=CurEvent->m_quadrant;
	MaxQ_2inchPMT=CurEvent->m_maxQ_2inchPMT;
	Column_2inchPMT=CurEvent->m_Column_2inchPMT;
	MiddleTimeRMS=CurEvent->m_MiddleTimeRMS;
	tRMS=CurEvent->m_tRMS;
	nPEMax=CurEvent->m_nPEMax;
	nPEMedian=CurEvent->m_nPEMedian;
	nPERMS=CurEvent->m_nPERMS;
	nPESum=CurEvent->m_nPESum;

	CalibReadout* calib = CurEvent->get<CalibReadout>("Event/CalibReadout/CalibReadoutHeader");
	nHitsRpc = calib->nHitsRpc;
	rpcRow = calib->rpcRow;
	rpcColumn = calib->rpcColumn;
	rpcLayer = calib->rpcLayer;
	/*
	RecRpcReadout* recrpc = CurEvent->get<RecRpcReadout>("Event/Rec/RpcSimple");
	eventType=recrpc->eventType;
	nClusters=recrpc->nClusters;
	std::vector<Cluster*>::iterator end = recrpc->clusters.end();
	for ( std::vector<Cluster*>::iterator it = recrpc->clusters.begin(); it != end; ++it ) { 
		cluster.push_back(*it);
	} 
	
	for ( vector<Cluster>::iterator it = recrpc->clusters.begin(); it != recrpc->clusters.end(); ++it ) {
		cluster.push_back(*it);	
	}
	*/
//	cluster.insert(recrpc->clusters.begin(),recrpc->clusters.end());

	AdScaledTree->Fill();
	CalibStatsTree->Fill();
	CalibReadoutHeaderTree->Fill();
	//RpcSimpleTree->Fill();

	return true;

}

bool SimplifiedRoot::finalize()
{
	return true;
}

