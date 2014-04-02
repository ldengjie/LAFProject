#include "LafKernel/PhyEvent.h"
#include "LafKernel/IPerEvent.h"

IPerEvent  *PhyEvent::s_perEvt = 0;
std::vector<std::string>  PhyEvent::m_inputFlist;

double operator-(const TTimeStamp& lhs, const TTimeStamp& rhs)
{
    return TTimeStamp(lhs.GetSec()     - rhs.GetSec(),
                     lhs.GetNanoSec() - rhs.GetNanoSec()).AsDouble();
}

PhyEvent* PhyEvent::GrabInstance()
{
    ++m_nHandler;
    return this;
}

void PhyEvent::ReleaseInstance()
{
    if ( --m_nHandler <= 0 ) {
	delete this;
    }
}

PhyEvent::PhyEvent()
    : m_entry(-1),
      m_good(true),
      m_trigTime(0, 0),
      m_flasherTag(false),
      m_2inchFlasherTag(false),
      m_rpcNoiseTag(false),
      m_forceTrigTag(false),
      m_crossTrigTag(false),
      m_adLowEnergyTag(false),
      m_muonTag(kNotTag),
      m_nHandler(0)
{
}

PhyEvent::~PhyEvent()
{
    std::map<std::string, DataSec*>::iterator end = m_secs.end();
    for ( std::map<std::string, DataSec*>::iterator it = m_secs.begin(); it != end; ++it ) {
	delete it->second;
    }
}

bool PhyEvent::isMuon()
{
   return (m_muonTag >= kWpMuon);
}

bool PhyEvent::isWpMuon()
{
   return (m_muonTag == kWpMuon);
}

bool PhyEvent::isRpcMuon()
{
   return (m_muonTag == kRpcMuon);
}

bool PhyEvent::isAdMuon()
{
   return (m_muonTag == kAdMuon);
}

bool PhyEvent::isShowerMuon()
{
   return (m_muonTag == kShowerMuon);
}
bool PhyEvent::isAdMuonNew()
{
   return (m_muonTag == kAdMuonNew);
}

bool PhyEvent::isShowerMuonNew()
{
   return (m_muonTag == kShowerMuonNew);
}
const std::string& PhyEvent::getFileName()
{
    return m_inputFlist[m_fileNum];
}
