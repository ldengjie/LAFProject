#include "PerPhyEvent.h"
#include "PerRecReadout.h"
#include "PerRecRpcReadout.h"
#include "PerCalibStats.h"
#include "PerCalibReadout.h"
#include "PerMuonRecSimple.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/PhyEvent/DataSec.h"
#include "LafKernel/LafException.h"
#include "TFile.h"
#include <iostream>

PerPhyEvent::PerPhyEvent(const std::vector<std::string>& infiles,
	                 const std::string& preferredRecTree,
			 const std::string& preferredEnergy)
    : fCurrent(-1),
      m_files(infiles)
{
    fRecReadoutChain = MakeChain(preferredRecTree.c_str());
    fCalibStatsChain = MakeChain("Event/Data/CalibStats");

    if ( preferredEnergy == "rawEvis" ) {
	_energy = &rawEvis;
    }
    else if ( preferredEnergy == "energy" ) {
	_energy = &energy;
    }
    else if ( preferredEnergy == "enrec" ) {
	_energy = &enrec;
    }
    else if ( preferredEnergy == "eprec" ) {
	_energy = &eprec;
    }
    else {
	std::string msg("ReconInputSvc: unknown preferredEnergy ");
	msg += preferredEnergy;
	throw LafException(msg);
    }

    Init();
    RegisterObj();

    link2PhyEvent(this);
}

PerPhyEvent::~PerPhyEvent()
{
    std::map<std::string, IPerDataSec*>::iterator it = m_topo.begin();
    while ( it != m_topo.end() ) {
	delete it->second;
	++it;
    }
    delete fRecReadoutChain->GetCurrentFile();
    link2PhyEvent(0);
}

PhyEvent* PerPhyEvent::makePhyEvent(Long64_t entry)
{
    PhyEvent* result = 0;
    if ( GetEntry(entry) > 0 ) {
	result = new PhyEvent();
	result->m_entry        = entry;
	result->m_localEntry   = fRecReadoutChain->GetTree()->GetReadEvent();
	result->m_fileNum      = fCurrent;
	//result->m_run          = this->Run;
	//result->m_event        = this->Event;
	result->m_site         = this->site;
	result->m_det          = this->detector;
	result->m_trigType     = this->triggerType;
	result->m_trigNum      = this->triggerNumber;
	result->m_trigTime     = TTimeStamp(this->triggerTimeSec, this->triggerTimeNanoSec);
	result->m_energy       = this->energy;
	result->m_rawEvis      = this->rawEvis;
	result->m_enrec        = this->enrec;
	result->m_eprec        = this->eprec;
	result->m_x            = this->x;
	result->m_y            = this->y;
	result->m_z            = this->z;
	result->m_nPmt         = this->NPmt;
	result->m_maxQ         = this->MaxQ;
	result->m_quadrant     = this->Quadrant;
	result->m_maxQ_2inchPMT = this->MaxQ_2inchPMT;
	result->m_Column_2inchPMT = this->Column_2inchPMT;
	result->m_MiddleTimeRMS = this->MiddleTimeRMS;
        result->m_nPEMax       = this->NPEMax;
        result->m_nPEMedian    = this->NPEMedian;
        result->m_nPERMS       = this->NPERMS;
        result->m_nPESum       = this->NPESum;
        result->m_tRMS         = this->TRMS;
        result->setEnergy(*(this->_energy));
    }
    return result;
}

PhyEvent*  PerPhyEvent::makePhyEvent(Int_t fNum, Long64_t entry)
{
    TChain* _chain = (TChain*)fRecReadoutChain;
    Long64_t gEntry = entry + (_chain->GetTreeOffset())[fNum];
    return makePhyEvent(gEntry);
}

DataSec* PerPhyEvent::getObj(const std::string& path, PhyEvent* pEvt)
{
    DataSec* obj = 0;
    std::map<std::string, IPerDataSec*>::iterator it = m_topo.find(path);
    if ( it != m_topo.end() ) {
	obj = it->second->ToTran(pEvt->m_entry, pEvt->m_fileNum, pEvt->m_localEntry);
    }
    else {
	std::string msg("Unknown Tree name: ");
	msg += path;
	throw LafException(msg);
    }
    return obj;
}

Int_t PerPhyEvent::GetEntry(Long64_t entry)
{
    // Read contents of entry.
    TChain* _chain = (TChain*)fRecReadoutChain;
    if ( _chain->LoadTree(entry) >=0 && _chain->GetTreeNumber() != fCurrent ) {
	fCurrent = _chain->GetTreeNumber();
	Notify();
    }
    fRecReadoutChain->GetEntry(entry);
    Int_t stat = fCalibStatsChain->GetEntry(entry);

    return (fCalibStatsChain->GetTreeNumber() == fCurrent) ? stat : -1;
}

void PerPhyEvent::Init()
{
   // Set branch addresses and branch pointers
   UInt_t nBr = 0;
   fRecReadoutChain->SetMakeClass(1);
   fRecReadoutChain->SetBranchStatus("*", 0, &nBr);
   fRecReadoutChain->SetBranchAddress("site", &site);
   fRecReadoutChain->SetBranchAddress("detector", &detector);
   fRecReadoutChain->SetBranchAddress("triggerType", &triggerType);
   fRecReadoutChain->SetBranchAddress("triggerNumber", &triggerNumber);
   fRecReadoutChain->SetBranchAddress("triggerTimeSec", &triggerTimeSec);
   fRecReadoutChain->SetBranchAddress("triggerTimeNanoSec", &triggerTimeNanoSec);
   fRecReadoutChain->SetBranchAddress("energy", &energy);
   fRecReadoutChain->SetBranchAddress("rawEvis", &rawEvis);
   fRecReadoutChain->SetBranchAddress("enrec", &enrec);
   fRecReadoutChain->SetBranchAddress("eprec", &eprec);
   fRecReadoutChain->SetBranchAddress("x", &x);
   fRecReadoutChain->SetBranchAddress("y", &y);
   fRecReadoutChain->SetBranchAddress("z", &z);

   fCalibStatsChain->SetMakeClass(1);
   fCalibStatsChain->SetBranchStatus("*", 0, &nBr);
   fCalibStatsChain->SetBranchAddress("nHit", &NPmt);
   fCalibStatsChain->SetBranchAddress("MaxQ", &MaxQ);
   fCalibStatsChain->SetBranchAddress("Quadrant", &Quadrant);
   fCalibStatsChain->SetBranchAddress("MaxQ_2inchPMT", &MaxQ_2inchPMT);
   fCalibStatsChain->SetBranchAddress("Column_2inchPMT", &Column_2inchPMT);
   fCalibStatsChain->SetBranchAddress("MiddleTimeRMS", &MiddleTimeRMS);
   fCalibStatsChain->SetBranchAddress("tRMS", &TRMS);
   fCalibStatsChain->SetBranchAddress("nPEMax", &NPEMax);
   fCalibStatsChain->SetBranchAddress("nPEMedian", &NPEMedian);
   fCalibStatsChain->SetBranchAddress("nPERMS", &NPERMS);
   fCalibStatsChain->SetBranchAddress("nPESum", &NPESum);
}

void PerPhyEvent::RegisterObj()
{
    char* path1 = "Event/Rec/AdSimple";
    IPerDataSec* obj1 = new PerRecReadout( MakeChain(path1) );
    m_topo.insert(std::make_pair<std::string, IPerDataSec*>(path1, obj1));

    char* path2 = "Event/Rec/AdScaled";
    IPerDataSec* obj2 = new PerRecReadout( MakeChain(path2) );
    m_topo.insert(std::make_pair<std::string, IPerDataSec*>(path2, obj2));

    char* path3 = "Event/Data/CalibStats";
    IPerDataSec* obj3 = new PerCalibStats( MakeChain(path3) );
    m_topo.insert(std::make_pair<std::string, IPerDataSec*>(path3, obj3));

    char* path4 = "Event/CalibReadout/CalibReadoutHeader";
    IPerDataSec* obj4 = new PerCalibReadout( MakeChain(path4) );
    m_topo.insert(std::make_pair<std::string, IPerDataSec*>(path4, obj4));

    char* path5 = "Event/Rec/RpcSimple";
    IPerDataSec* obj5 = new PerRecRpcReadout( MakeChain(path5) );
    m_topo.insert(std::make_pair<std::string, IPerDataSec*>(path5, obj5));

    char* path6 = "Event/Rec/PoolSimple";
    IPerDataSec* obj6 = new PerRecReadout( MakeChain(path6) );
    m_topo.insert(std::make_pair<std::string, IPerDataSec*>(path6, obj6));

    char* path7 = "Event/Rec/AdMLRec";
    IPerDataSec* obj7 = new PerRecReadout( MakeChain(path7) );
    m_topo.insert(std::make_pair<std::string, IPerDataSec*>(path7, obj7));

    char* path8 = "Event/Data/Physics/MuonRecSimple";
    IPerDataSec* obj8 = new PerMuonRecSimple( MakeChain(path8) );
    m_topo.insert(std::make_pair<std::string, IPerDataSec*>(path8, obj8));
}

Bool_t PerPhyEvent::Notify()
{
    TChain* _chain = (TChain*)fRecReadoutChain;
    std::cout << "Processing " << _chain->GetFile()->GetName() << std::endl;
    return kTRUE;
}

TChain* PerPhyEvent::MakeChain(const char* name)
{
    TChain* chain = new TChain(name, name);
    for ( std::vector<std::string>::const_iterator it = m_files.begin(); it != m_files.end(); ++it) {
	chain->AddFile(it->c_str());
    }
    if ( chain->GetNtrees() != (int)m_files.size() ) {
	delete chain;
	std::string msg("Failed to make TChain: ");
	msg += name;
	throw LafException(msg);
    }
    return chain;
}
