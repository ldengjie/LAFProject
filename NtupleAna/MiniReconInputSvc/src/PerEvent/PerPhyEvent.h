#ifndef PER_PHY_EVENT_H
#define PER_PHY_EVENT_H

#include "LafKernel/IPerEvent.h"
#include <TChain.h>

class IPerDataSec;
class PhyEvent;

class PerPhyEvent : public IPerEvent
{
    public :

	PerPhyEvent(const std::vector<std::string>& infiles,
		    const std::string& preferredRecTree,
		    const std::string& preferredEnergy);
	virtual ~PerPhyEvent();

	PhyEvent*  makePhyEvent(Long64_t entry);
	PhyEvent*  makePhyEvent(Int_t fNum, Long64_t entry);

    protected :

	virtual DataSec* getObj(const std::string& path, PhyEvent* pEvt);

	Int_t    GetEntry(Long64_t entry);
	void     Init();
	void     RegisterObj();
	Bool_t   Notify();
	TChain*  MakeChain(const char* name);

	TTree*          fRecReadoutChain;
	TTree*          fCalibStatsChain;

	Int_t           fCurrent; //!current Tree number in a TChain

	// Declaration of leaf types
	Short_t         site;
	Short_t         detector;
	UInt_t          triggerType;
	Int_t           triggerNumber;
	Int_t           triggerTimeSec;
	Int_t           triggerTimeNanoSec;
	Float_t         energy;
	Float_t         rawEvis;
	Float_t         enrec;
	Float_t         eprec;
	Float_t         x;
	Float_t         y;
	Float_t         z;
	Int_t           NPmt;  //nHit in CalibStats
	Float_t         MaxQ;
	Float_t         Quadrant;
	Float_t         MaxQ_2inchPMT;
	Int_t           Column_2inchPMT;
	Float_t         MiddleTimeRMS;
        Float_t         TRMS; // tRMS in calibStats
        Float_t         NPEMax;
        Float_t         NPEMedian;
        Float_t         NPERMS;
        Float_t         NPESum;

	Float_t*        _energy;

	std::map<std::string, IPerDataSec*> m_topo;
	const std::vector<std::string>& m_files;
};

#endif
