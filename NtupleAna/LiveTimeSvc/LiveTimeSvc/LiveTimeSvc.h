#ifndef LIVE_TIME_SVC_H
#define LIVE_TIME_SVC_H

#include "LafKernel/SvcBase.h"
#include "TTimeStamp.h"
#include <string>

class TH1F;
class TTree;

class LiveTimeSvc : public SvcBase
{
  public :

    LiveTimeSvc(const std::string& name);
    virtual ~LiveTimeSvc();

    virtual bool initialize();
    void initHist();
    virtual bool finalize();

    inline double wallTime() const { return m_wallTime; }
    inline double daqTime() const { return m_daqTime; }
    inline double liveTime(const int det) const { return m_liveTime[det-1]; }
    inline double liveTimeOld(const int det) const { return m_liveTimeOld[det-1]; }
    inline double liveTime4IbdSel(const int det) const { return m_liveTime4IbdSel[det-1]; }
    inline double liveTime4IbdSelOld(const int det) const { return m_liveTime4IbdSelOld[det-1]; }
    inline double evtTimeGap() const { return m_evtTimeGap; }

    inline void setWallTime(const double wallTime) { m_wallTime = wallTime; }
    inline void setDaqTime(const double daqTime) { m_daqTime = daqTime; }
    inline void setFirstGapEventTrigtime(const double trigtime){m_firstGapEventTrigtime=trigtime;}
    inline void setLastGapEventTrigtime(const double trigtime){m_lastGapEventTrigtime=trigtime;}
    inline double getFirstGapEventTrigtime(){return m_firstGapEventTrigtime;}
    inline double getLastGapEventTrigtime(){return m_lastGapEventTrigtime;}
    inline void setNumberOfSingleLiveTime200(const int det,int number){NumberOfSingleLiveTime200[det-1]=number;}
    inline void setNumberOfSingleLiveTime200Old(const int det,int number){NumberOfSingleLiveTime200Old[det-1]=number;}
    inline int getNumberOfSingleLiveTime200(const int det){return NumberOfSingleLiveTime200[det-1];}
    inline int getNumberOfSingleLiveTime200Old(const int det){return NumberOfSingleLiveTime200Old[det-1];}
    inline void setLiveTime(const int det, const double liveTime) { m_liveTime[det-1] = liveTime; }
    inline void setLiveTimeOld(const int det, const double liveTime) { m_liveTimeOld[det-1] = liveTime; }
    inline void setLiveTime4IbdSel(const int det, const double liveTime) { m_liveTime4IbdSel[det-1] = liveTime; }
    inline void setLiveTime4IbdSelOld(const int det, const double liveTime) { m_liveTime4IbdSelOld[det-1] = liveTime; }
    inline void setEvtTimeGap(const double evtTimeGap) { m_evtTimeGap = evtTimeGap; }

    inline TTimeStamp& startTime() { return m_startTime; }

    inline TTimeStamp& endTime() { return m_endTime; }
    inline int nBins() const { return m_nBins; }

    double daqTimePerBin(const int bin);
    double liveTimePerBin(const int det, const int bin);
    double liveTimePerBinOld(const int det, const int bin);
    double liveTimePerBin4IbdSel(const int det, const int bin);
    double liveTimePerBin4IbdSelOld(const int det, const int bin);

    void setDaqTimePerBin(const int bin, const double daqTime);
    void setLiveTimePerBin(const int det, const int bin, const double liveTime);
    void setLiveTimePerBinOld(const int det, const int bin, const double liveTime);
    void setLiveTimePerBin4IbdSel(const int det, const int bin, const double liveTime);
    void setLiveTimePerBin4IbdSelOld(const int det, const int bin, const double liveTime);
    void setSingleLiveTimePerBin(const int det, const int bin, const double liveTime);
    void setSingleLiveTimePerBinOld(const int det, const int bin, const double liveTime);
    void setSingleLiveTimePerBin200(const int det, const int bin, const double liveTime);
    void setSingleLiveTimePerBin200Old(const int det, const int bin, const double liveTime);
    void setNumOfSingleLiveTime200(const int det, const int bin);
    void setNumOfSingleLiveTime200Old(const int det, const int bin);
    void fillSingleLiveTimeTree(const int det,const int bin,const double liveTime);
    void fillSingleLiveTimeTreeOld(const int det,const int bin,const double liveTime);

    inline double binWidth() const { return m_binWidth; }
    int bin(TTimeStamp &time) const;
    TTimeStamp startTime(const int bin) const;
    TTimeStamp endTime(const int bin) const;

    inline TH1F* hDaqTime() { return m_hDaqTime; }
    inline TH1F* hLiveTime(const int det) { return m_hLiveTime[det-1]; }
    inline TH1F* hLiveTimeOld(const int det) { return m_hLiveTimeOld[det-1]; }

    inline TH1F* hLiveTime4IbdSel(const int det) { return m_hLiveTime4IbdSel[det-1]; }
    inline TH1F* hLiveTime4IbdSelOld(const int det) { return m_hLiveTime4IbdSelOld[det-1]; }
    inline TH1F* hSingleLiveTime(const int det) { return m_hSingleLiveTime[det-1]; }
    inline TH1F* hSingleLiveTimeOld(const int det) { return m_hSingleLiveTimeOld[det-1]; }
    inline TH1F* hSingleLiveTime200(const int det) { return m_hSingleLiveTime200[det-1]; }
    inline TH1F* hSingleLiveTime200Old(const int det) { return m_hSingleLiveTime200Old[det-1]; }
    inline TH1F* hNumOfSingleLiveTime200(const int det) { return m_hNumOfSingleLiveTime200[det-1]; }
    inline TH1F* hNumOfSingleLiveTime200Old(const int det) { return m_hNumOfSingleLiveTime200Old[det-1]; }
    inline TTree* tSingleLiveTimeWindow(){return m_tSingleLiveTimeWindow;}
    inline TTree* tSingleLiveTimeWindowOld(){return m_tSingleLiveTimeWindowOld;}
  private:

    int str2int(const std::string &str);

  private :

    double  m_wallTime; // DAQ data taking time include time gap.
    double  m_daqTime;  // DAQ data taking time. Time gap is excluded.
    double  m_liveTime[4]; // Currently muon veto time is excluded.
    double  m_liveTimeOld[4]; // Currently muon veto time is excluded.
    double  m_liveTime4IbdSel[4]; // Currently muon veto time is excluded.
    double  m_liveTime4IbdSelOld[4]; // Currently muon veto time is excluded.
    double  m_evtTimeGap; // time gap definition
    double  m_firstGapEventTrigtime;
    double  m_lastGapEventTrigtime;
    int NumberOfSingleLiveTime200[4];
    int NumberOfSingleLiveTime200Old[4];
    int binNum;
    double liveTimeWindow; 
    int m_det;

    // For VS time figures
    TTimeStamp m_startTime; // Start data taking time
    TTimeStamp m_endTime; // End data taking time
    int m_nBins;  // Number of bins
    std::string m_startTimeStr;  
    std::string m_endTimeStr;  
    double m_binWidth; // Width of a bin in second

  private:
    TH1F* m_hDaqTime;
    TH1F* m_hLiveTime[4];
    TH1F* m_hLiveTimeOld[4];
    TH1F* m_hLiveTime4IbdSel[4];
    TH1F* m_hLiveTime4IbdSelOld[4];
    TH1F* m_hSingleLiveTime[4];
    TH1F* m_hSingleLiveTimeOld[4];
    TH1F* m_hSingleLiveTime200[4];
    TH1F* m_hSingleLiveTime200Old[4];
    TH1F* m_hNumOfSingleLiveTime200[4];
    TH1F* m_hNumOfSingleLiveTime200Old[4];
    TTree* m_tSingleLiveTimeWindowOld;
    TTree* m_tSingleLiveTimeWindow;

};

#endif
