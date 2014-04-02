#include "LiveTimeSvc/LiveTimeSvc.h"
#include "LafKernel/SvcFactory.h"
#include "LafKernel/GlobalVar.h"
#include <fstream>
#include <sstream>
#include "TH1F.h"
#include "TTree.h"

using namespace std;

DECLARE_SERVICE(LiveTimeSvc);

LiveTimeSvc::LiveTimeSvc(const std::string& name)
: SvcBase(name),
  m_wallTime(0.),
  m_daqTime(0.),
  m_startTime(0),
  m_endTime(0),
  m_nBins(100),
  m_binWidth(0.)
{
  m_name = "LiveTimeSvc";  //to avoid naming confusion
  setOption("TimeGap",       m_evtTimeGap=1.); // 1 second
  setOption("StartTime",     m_startTimeStr="2011-08-15T00:00:00");
  setOption("EndTime",       m_endTimeStr="2012-08-15T00:00:00");
  setOption("NBins",         m_nBins=2196);
  setOption("BinWidth",      m_binWidth=0.);

  string strYear1(m_startTimeStr.substr(0,4));
  string strMonth1(m_startTimeStr.substr(5,2));
  string strDay1(m_startTimeStr.substr(8,2));
  string strHour1("00");
  string strMinute1("00");
  string strSecond1("00");

  if(m_startTimeStr.length() >= 13) {
    strHour1 = m_startTimeStr.substr(11,2);
  }
  if(m_startTimeStr.length() >= 16) {
    strMinute1 = m_startTimeStr.substr(14,2);
  }
  if(m_startTimeStr.length() >= 19) {
    strSecond1 = m_startTimeStr.substr(17,2);
  }

  string strYear2(m_endTimeStr.substr(0,4));
  string strMonth2(m_endTimeStr.substr(5,2));
  string strDay2(m_endTimeStr.substr(8,2));
  string strHour2("00");
  string strMinute2("00");
  string strSecond2("00");

  if(m_endTimeStr.length() >= 13) {
    strHour2 = m_endTimeStr.substr(11,2);
  }
  if(m_endTimeStr.length() >= 16) {
    strMinute2 = m_endTimeStr.substr(14,2);
  }
  if(m_endTimeStr.length() >= 19) {
    strSecond2 = m_endTimeStr.substr(17,2);
  }

  m_startTime = TTimeStamp(str2int(strYear1),str2int(strMonth1),str2int(strDay1),
      str2int(strHour1),str2int(strMinute1),str2int(strSecond1));
  m_endTime = TTimeStamp(str2int(strYear2),str2int(strMonth2),str2int(strDay2),
      str2int(strHour2),str2int(strMinute2),str2int(strSecond2));

  if(m_binWidth>0.) {
    m_nBins = (int)((m_endTime-m_startTime)/m_binWidth);
  } else {
    m_binWidth = (m_endTime-m_startTime)/(double)m_nBins;
  }

  LogInfo << "Time range for \"VS time\" histogram with " << m_nBins << " bins:" << endl;
  LogInfo << "Start time: " << m_startTime << endl;
  LogInfo << "End time: " << m_endTime << endl;
  LogInfo << "Bin width: " << binWidth() << "s" << endl;

  for(int det=1;det<=4;det++) {
    m_liveTime[det-1] = 0.;
    m_liveTimeOld[det-1] = 0.;
    m_liveTime4IbdSel[det-1] = 0.;
    m_liveTime4IbdSelOld[det-1] = 0.;
  }

}

void LiveTimeSvc::initHist()
{
  m_hDaqTime = new TH1F("DaqTime","DAQ time",
      m_nBins, m_startTime.AsDouble(), m_endTime.AsDouble()
      );
  m_tSingleLiveTimeWindow = new TTree("SingleLiveTimeWindow","SingleLiveTimeWindow");
  m_tSingleLiveTimeWindow->Branch("det",&m_det,"m_det/I");
  m_tSingleLiveTimeWindow->Branch("binNum",&binNum,"binNum/I");
  m_tSingleLiveTimeWindow->Branch("liveTimeWindow",&liveTimeWindow,"liveTimeWindow/D");
  m_tSingleLiveTimeWindowOld = new TTree("SingleLiveTimeWindowOld","SingleLiveTimeWindowOld");
  m_tSingleLiveTimeWindowOld->Branch("det",&m_det,"m_det/I");
  m_tSingleLiveTimeWindowOld->Branch("binNum",&binNum,"binNum/I");
  m_tSingleLiveTimeWindowOld->Branch("liveTimeWindow",&liveTimeWindow,"liveTimeWindow/D");
  for(int det=1;det<=GlobalVar::NumADs;det++) {
    stringstream ss;
    ss << "AD" << det << "LiveTime";
    m_hLiveTime[det-1] = new TH1F(ss.str().c_str(),"Live time", m_nBins, m_startTime.AsDouble(), m_endTime.AsDouble() );
	ss<<"0";
    m_hLiveTimeOld[det-1] = new TH1F(ss.str().c_str(),"Live time", m_nBins, m_startTime.AsDouble(), m_endTime.AsDouble() );

    stringstream ss4IbdSel;
    ss4IbdSel << "AD" << det << "LiveTime4IbdSel";
    m_hLiveTime4IbdSel[det-1] = new TH1F(ss4IbdSel.str().c_str(),"Live time", m_nBins, m_startTime.AsDouble(), m_endTime.AsDouble() );
	ss4IbdSel<<"0";
    m_hLiveTime4IbdSelOld[det-1] = new TH1F(ss4IbdSel.str().c_str(),"Live time", m_nBins, m_startTime.AsDouble(), m_endTime.AsDouble() );

    stringstream ssSingle;
    ssSingle << "AD" << det << "SingleLiveTime";
    m_hSingleLiveTime[det-1] = new TH1F(ssSingle.str().c_str(),"Live time", m_nBins, m_startTime.AsDouble(), m_endTime.AsDouble() );
	ssSingle<<"0";
    m_hSingleLiveTimeOld[det-1] = new TH1F(ssSingle.str().c_str(),"Live time", m_nBins, m_startTime.AsDouble(), m_endTime.AsDouble() );
    stringstream ssNumOfSingle;
    ssNumOfSingle << "AD" << det << "NumOfSingleLiveTime200";
    m_hNumOfSingleLiveTime200[det-1] = new TH1F(ssNumOfSingle.str().c_str(),"Live time", m_nBins, m_startTime.AsDouble(), m_endTime.AsDouble() );
	ssNumOfSingle<<"0";
    m_hNumOfSingleLiveTime200Old[det-1] = new TH1F(ssNumOfSingle.str().c_str(),"Live time", m_nBins, m_startTime.AsDouble(), m_endTime.AsDouble() );
    stringstream ssSingle200;
    ssSingle200 << "AD" << det << "SingleLiveTime200";
    m_hSingleLiveTime200[det-1] = new TH1F(ssSingle200.str().c_str(),"Live time", m_nBins, m_startTime.AsDouble(), m_endTime.AsDouble() );
	ssSingle200<<"0";
    m_hSingleLiveTime200Old[det-1] = new TH1F(ssSingle200.str().c_str(),"Live time", m_nBins, m_startTime.AsDouble(), m_endTime.AsDouble() );
  }
}

LiveTimeSvc::~LiveTimeSvc()
{
}

bool LiveTimeSvc::initialize()
{
  return true;
}

bool LiveTimeSvc::finalize()
{
  return true;
}

int LiveTimeSvc::bin(TTimeStamp &time) const
{
  return (int)((time - m_startTime)/binWidth());
}

TTimeStamp LiveTimeSvc::startTime(const int bin) const
{
  TTimeStamp time(m_startTime);
  int ts = (int)(bin*binWidth());
  int tns = (int)((bin*binWidth() - ts)*1e9);
  TTimeStamp dt(ts,tns);
  time.Add(dt);
  return time;
}

TTimeStamp LiveTimeSvc::endTime(const int bin) const
{
  return startTime(bin+1);
}

int LiveTimeSvc::str2int(const std::string &str)
{
  stringstream ss;
  ss << str;
  int temp;
  ss >> temp;
  return temp;
}

double LiveTimeSvc::daqTimePerBin(const int bin) 
{ 
  return m_hDaqTime->GetBinContent(bin+1); 
}

double LiveTimeSvc::liveTimePerBin(const int det, const int bin) 
{ 
  return m_hLiveTime[det-1]->GetBinContent(bin+1); 
}
double LiveTimeSvc::liveTimePerBinOld(const int det, const int bin) 
{ 
  return m_hLiveTimeOld[det-1]->GetBinContent(bin+1); 
}

double LiveTimeSvc::liveTimePerBin4IbdSel(const int det, const int bin) 
{ 
  return m_hLiveTime4IbdSel[det-1]->GetBinContent(bin+1); 
}
double LiveTimeSvc::liveTimePerBin4IbdSelOld(const int det, const int bin) 
{ 
  return m_hLiveTime4IbdSelOld[det-1]->GetBinContent(bin+1); 
}

void LiveTimeSvc::setDaqTimePerBin(const int bin, const double daqTime) 
{ 
  m_hDaqTime->Fill(startTime(bin) + binWidth()/2, daqTime);
}

void LiveTimeSvc::setLiveTimePerBin(const int det, const int bin, const double liveTime) 
{ 
  m_hLiveTime[det-1]->Fill(startTime(bin) + binWidth()/2, liveTime);
}
void LiveTimeSvc::setLiveTimePerBinOld(const int det, const int bin, const double liveTime) 
{ 
  m_hLiveTimeOld[det-1]->Fill(startTime(bin) + binWidth()/2, liveTime);
}
void LiveTimeSvc::setLiveTimePerBin4IbdSel(const int det, const int bin, const double liveTime) 
{ 
  m_hLiveTime4IbdSel[det-1]->Fill(startTime(bin) + binWidth()/2, liveTime);
}
void LiveTimeSvc::setLiveTimePerBin4IbdSelOld(const int det, const int bin, const double liveTime) 
{ 
  m_hLiveTime4IbdSelOld[det-1]->Fill(startTime(bin) + binWidth()/2, liveTime);
}

void LiveTimeSvc::setSingleLiveTimePerBin(const int det, const int bin, const double liveTime) 
{ 
  m_hSingleLiveTime[det-1]->Fill(startTime(bin) + binWidth()/2, liveTime);
}
void LiveTimeSvc::setSingleLiveTimePerBinOld(const int det, const int bin, const double liveTime) 
{ 
  m_hSingleLiveTimeOld[det-1]->Fill(startTime(bin) + binWidth()/2, liveTime);
}
void LiveTimeSvc::setSingleLiveTimePerBin200(const int det, const int bin, const double liveTime) 
{ 
  m_hSingleLiveTime200[det-1]->Fill(startTime(bin) + binWidth()/2, liveTime);
}
void LiveTimeSvc::setSingleLiveTimePerBin200Old(const int det, const int bin, const double liveTime) 
{ 
  m_hSingleLiveTime200Old[det-1]->Fill(startTime(bin) + binWidth()/2, liveTime);
}
void LiveTimeSvc::setNumOfSingleLiveTime200(const int det, const int bin) 
{ 
  m_hNumOfSingleLiveTime200[det-1]->SetBinContent(bin,NumberOfSingleLiveTime200[det-1]);
}
void LiveTimeSvc::setNumOfSingleLiveTime200Old(const int det,const int bin) 
{ 
  m_hNumOfSingleLiveTime200Old[det-1]->SetBinContent(bin,NumberOfSingleLiveTime200Old[det-1]);
}
void LiveTimeSvc::fillSingleLiveTimeTree(const int det,const int bin,const double liveTime)
{
    m_det=det;
    binNum=bin;
    liveTimeWindow=liveTime;
    m_tSingleLiveTimeWindow->Fill();
}
void LiveTimeSvc::fillSingleLiveTimeTreeOld(const int det,const int bin,const double liveTime)
{
    m_det=det;
    binNum=bin;
    liveTimeWindow=liveTime;
    m_tSingleLiveTimeWindowOld->Fill();
}
