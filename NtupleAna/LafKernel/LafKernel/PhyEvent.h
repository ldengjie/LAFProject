#ifndef PHYEVENT_H
#define PHYEVENT_H

#include "LafKernel/IPerEvent.h"
#include "LafKernel/PhyEvent/DataSec.h"
#include "LafKernel/EvtObject.h"
#include "TTimeStamp.h"
#include <vector>
#include <map>
#include <string>

double operator- (const TTimeStamp &lhs, const TTimeStamp &rhs);

class PhyEvent : public EvtObject
{
    public :

	PhyEvent* GrabInstance();
	void      ReleaseInstance();

	PhyEvent();

	template <typename Type>
	Type* get(const std::string& path);

	bool tagMuon()      { return (m_muonTag&kNotTag) == 0; }
	bool isMuon();
	bool isWpMuon();
	bool isRpcMuon();
	bool isAdMuon();
	bool isShowerMuon();
	bool isAdMuonNew();
	bool isShowerMuonNew();

	bool isAD()  { return m_det<=4&&m_det>=1; }
	bool isWP()  { return m_det==5||m_det==6; }
	bool isRPC() { return m_det==7; }

	// global entry in TChain
	long        m_entry;
	// local entry and file number
	long        m_localEntry;
	int         m_fileNum;

	// good event flag based on tags
	bool        m_good;

	// event context
	// int         m_run;
	// int         m_event;
	int         m_site;
	int         m_det;
	int         m_trigType;
	int         m_trigNum;
	TTimeStamp  m_trigTime;

	// most frequently used info (from RecData)

	float       energy() {  //the preferred energy that configured in InputSvc
	    return m_preferredEnergy;
	}

	float       m_energy;
	float       m_rawEvis;
	float       m_enrec;
	float       m_eprec;
	float       m_x;
	float       m_y;
	float       m_z;
	int         m_nPmt;  //nHit in CalibStats
	float       m_maxQ;
	float       m_quadrant;
	float       m_maxQ_2inchPMT;
	int         m_Column_2inchPMT;
	float       m_MiddleTimeRMS;
        float       m_tRMS;
        float       m_nPEMax;
        float       m_nPEMedian;
        float       m_nPERMS;
        float       m_nPESum;
	// tags
	bool        m_flasherTag;
	bool        m_2inchFlasherTag;
	bool        m_rpcNoiseTag;
	bool        m_forceTrigTag;
	bool        m_crossTrigTag;
	bool        m_adLowEnergyTag;
	int         m_muonTag;

	std::map<std::string, float> m_userTag;

	// constants of tags
	static const int kNotTag      = 0x1;
	static const int kNotMuon     = 0x2;
	static const int kWpMuon      = 0x4;
	static const int kRpcMuon     = 0x8;
	static const int kAdMuon      = 0x10;
	static const int kShowerMuon  = 0x20;
	static const int kAdMuonNew      = 0x11;
	static const int kShowerMuonNew  = 0x21;

	void   setEnergy(float energy) { m_preferredEnergy = energy; }

	const std::string& getFileName();
	static std::vector<std::string>  m_inputFlist;

    protected :

	float  m_preferredEnergy;
	std::map<std::string, DataSec*> m_secs;

	// the pointer to the persistent event data model
	friend class IPerEvent;
	static IPerEvent  *s_perEvt;


    private :

	// for automatic memory management
	int     m_nHandler;
	virtual ~PhyEvent();

	PhyEvent(const PhyEvent&);  //Do not supported
	PhyEvent& operator=(const PhyEvent&);  //Do not supported
};

template <typename Type>
Type* PhyEvent::get(const std::string& path)
{
    Type* result = 0;
    std::map<std::string, DataSec*>::iterator it = m_secs.find(path);
    if ( it != m_secs.end() ) {
	result = static_cast<Type*>(it->second);
    }
    else {
	DataSec* obj = s_perEvt->getObj(path, this);
	result = static_cast<Type*>(obj);
	m_secs.insert(std::make_pair<std::string, DataSec*>(path, obj));
    }
    return result;
}

#endif
