#include "TimeShiftAlg.h"
#include "LafKernel/PhyEvent/RecRpcReadout.h"
#include "LafKernel/UserBufMgr.h"
#include "LafKernel/AlgFactory.h"
#include "TCanvas.h"
#include <fstream>
#include <iomanip>
#include <sys/stat.h>
#include <sys/types.h>

DECLARE_ALGORITHM(TimeShiftAlg);

TimeShiftAlg::TimeShiftAlg(const std::string& name)
    : AlgBase(name),
      m_site(-1)
{
    setOption("TimeWindow", m_tw = 1000);
    setOption("StandardDet", m_stdDet = 5);
    setOption("MultiDet",  m_mulCut = 3);
    setOption("SavePlot",  m_savePlot = false);
    setOption("OutputPrefix",  m_rpath = "");
}

TimeShiftAlg::~TimeShiftAlg()
{
}

bool TimeShiftAlg::initialize()
{
    m_buffer = dynamic_cast<PhyEventBuf*>(service("Cycler"));
    m_muonBuf = UserBufMgr::bufWithSelector("MuonSelector");

    char* detName[7] = { "AD1", "AD2", "AD3", "AD4", "IWS", "OWS", "RPC" };
    for ( int i = 0; i < 7; ++i ) {
	if ( i == (m_stdDet-1) ) {
	    m_hist[i] = 0;
	    continue;
	}
	std::string hName = std::string(detName[i]) + "#rightarrow" + detName[m_stdDet-1];
	m_hist[i] = new TH1I(hName.c_str(), hName.c_str(), 2*m_tw, -m_tw, m_tw);
    }
    return true;
}

bool TimeShiftAlg::execute()
{
    if ( m_muonBuf->totalSize() < 3 ) return true;

    PhyEvent *event = m_muonBuf->curEvt();
    LogDebug << "Muon Buffer size: " << m_muonBuf->totalSize() 
	          << "  Det-" << event->m_det << std::endl;
    if ( event != m_buffer->curEvt() || event->m_det != m_stdDet ) return true;

    int multi = 1;
    int dt[7] = { 9999, 9999, 9999, 9999, 9999, 9999, 9999 };

    for ( PhyEventBuf::Iterator it = m_muonBuf->begin(); it < m_muonBuf->end(); ++it ) {
	LogDebug << "Detector-" << (*it)->m_det << std::endl;
	if ( *it == event ) continue;

	int iDt = int(((*it)->m_trigTime - event->m_trigTime) * 1.e9);

	if ( iDt <= -m_tw ) continue;
	if ( iDt >= m_tw ) break;

	int iDet = (*it)->m_det - 1;
	if ( iDet == (event->m_det-1) || dt[iDet] != 9999 ) continue ;

	dt[iDet] = iDt;
	++multi;
    }

    if ( multi >= m_mulCut ) {
	for ( int i = 0; i < 7; ++i ) {
	    if ( dt[i] != 9999 ) {
		m_hist[i]->Fill(dt[i]);
	    }
	}
    }

    if ( m_site < 0 ) m_site = event->m_site;

    return true;
}

bool TimeShiftAlg::finalize()
{
    std::string pshift = m_rpath;
    if ( m_rpath.empty() ) {
	pshift = "TimeShift";
    }

    mkdir(pshift.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );

    if ( m_savePlot ) {
	TCanvas cav("cav", "cav", 1200, 640);
	cav.Divide(3, 2);

	int iPad = 0;
	for ( int i = 0; i < 7; ++i ) {
	    if ( m_hist[i] != 0 ) {
		cav.cd(++iPad);
		m_hist[i]->Draw();
	    }
	}

	cav.Print( (pshift + "/" + pshift + ".png").c_str() );
    }

    char* siteName[5] = { "Unknown", "DayaBay", "LingAo", "Keep", "Far" };
    char* detName[7]  = { "AD1", "AD2", "AD3", "AD4", "IWS", "OWS", "RPC" };

    for ( int i = 0; i < 7; ++i ) {
	double mean = 0.0;
	if ( m_hist[i] != 0 && m_hist[i]->GetEntries() != 0 ) {
	    int mid = m_hist[i]->GetMaximumBin();
	    m_hist[i]->GetXaxis()->SetRange(mid-100, mid+100);
	    mean = m_hist[i]->GetMean();
	    delete m_hist[i];
	}

	std::ofstream ofs( (pshift + '/' + siteName[m_site] + '_' + detName[i] + "_Data.csv").c_str() );
	ofs << "#Table  of  Time Latency between detectors" << std::endl
	    << "#[Latency]" << std::endl
	    << std::setiosflags(std::ios::fixed) << std::setprecision(1)
	    << mean
	    << std::endl;
    }

    return true;
}
