#include "MuonTag.h"
#include "LafKernel/PhyEvent/RecRpcReadout.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/PhyEvent/RecReadout.h"
#include "LafKernel/ToolFactory.h"
#include "LafKernel/PhyEvent/CalibReadout.h"
#include  <iostream>
#include  <sstream>
#include  <map>
#include  <string>
using namespace std;
DECLARE_TOOL(MuonTag);

MuonTag::MuonTag(const std::string& name)
    : ITagBase(name)
{
  setOption("ApplyVeto",m_veto=false);
  setOption("AdMuonThreshold",m_adMuonThr);
  setOption("ShowerMuonThreshold",m_showerMuonThr);
  setOption("IwsNPmtThreshold",m_iwsNpmtThr);
  setOption("OwsNPmtThreshold",m_owsNpmtThr);

  OptionParser::setOption(name, "tagRPCMuon", m_tagRPCMuon=true);
  OptionParser::setOption(name, "tagADMuon", m_tagADMuon=true);
}

bool MuonTag::tag(PhyEvent *event)
{
  if(!event->m_good) {
    return false;
  }

  if(event->m_det==5) { // IWS
    if(event->m_nPmt>m_iwsNpmtThr) {
      event->m_muonTag=PhyEvent::kWpMuon;
      return m_veto;
    }
  } else if (event->m_det==6) { // OWS
    if(event->m_nPmt>m_owsNpmtThr) {
      event->m_muonTag=PhyEvent::kWpMuon;
      return m_veto;
    }
  } else if (event->m_det==7) {
    if(m_tagRPCMuon && ((event->m_trigType&0x00040000) != 0) ) {  // RPC 3/4
		CalibReadout* calib = event->get<CalibReadout>("Event/CalibReadout/CalibReadoutHeader");
		int nHits = calib->nHitsRpc;
		bool IsRpc=true;
		map<string,int> strip_hitnumber;
		map<string,int>::iterator it;
		int row;
		int col;
		int layer;
		ostringstream oss;
		string s;
		for( int i=0 ; i<nHits ; i++ )
		{
			row =  (calib->rpcRow).at(i);
			col =  (calib->rpcColumn).at(i);
			layer = (calib->rpcLayer).at(i);
			oss<<row;
			oss<<col;
			oss<<layer;
			s=oss.str();
			it=strip_hitnumber.find(s);
			if( it==strip_hitnumber.end() )
			{
				strip_hitnumber[s]=1;
			} else
			{
				strip_hitnumber[s]++;
			}
			oss.str("");

		}
		for( it=strip_hitnumber.begin() ; it!=strip_hitnumber.end() ; it++ )
		{
			if( it->second>2 )//
			{
				IsRpc=false;
				break;
			}
		}
		
		if( IsRpc )
		{
			event->m_muonTag=PhyEvent::kRpcMuon;	
		}

      //event->m_muonTag=PhyEvent::kRpcMuon;
      return m_veto;
    }
  } else {
    if(event->energy()>m_adMuonThr && m_tagADMuon) {
      event->m_muonTag=PhyEvent::kAdMuon;
      if(event->energy()>m_showerMuonThr) {
        event->m_muonTag=PhyEvent::kShowerMuon;
      }
      return m_veto;
    }
  }
  return false;
}
