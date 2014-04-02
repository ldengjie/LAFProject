#include "QMLFTool.h"

#include "DetHelpers/IPmtGeomInfoSvc.h"
#include "DetHelpers/ICoordSysSvc.h"
#include "DataSvc/ICalibDataSvc.h"
#include "Conventions/Detectors.h"
#include "Conventions/Reconstruction.h"
#include "Event/RecTrigger.h"
#include "Event/CalibReadoutPmtCrate.h"

#include "TMinuit.h"
#include "TGraph.h"
#include "TMath.h"

#include "DetDesc/Material.h"
#include "DetDesc/Surface.h"
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/TabulatedProperty.h"
#include <fstream>
#include <cstdlib>

#define NREF_ORDER 3

//int  tttttttt = 0;
QMLFTool* QMLFTool::current_instance = 0;

void QMLFTool::NCLL_FCN(int& npar, double* grad, double& fval, double* xval, int iflag)
{
    current_instance->ncll_fcn(npar, grad, fval, xval, iflag);
}

QMLFTool::QMLFTool(const std::string& type,
	           const std::string& name,
		   const IInterface* parent)
  : GaudiTool(type, name, parent),
    m_opPara(1.25E+04, 0.98, 0.98, 9000, 3.14*103*103),
    m_geomPara(2010., -2010.)
{
    declareInterface<IReconTool>(this);

    declareProperty("PmtGeomSvcName", m_pmtGeomSvcName = "PmtGeomInfoSvc", 
	    "Name of Pmt Geometry Information Service");
    declareProperty("PmtCalibDataSvcName", m_pmtCalibDataSvcName = "CalibDataSvc", 
	    "Name of Pmt Calib Data Information Service");

    // declare property for those optical parameters
    declareProperty("LLFMode", m_LLFMode = 1, "Likelihood function mode");
    declareProperty("addTimeLLF", m_addTimeLLF = false, "Add time likelihood or not");
    declareProperty("pmtChgCut", m_pmtChgCut = 12., "The charge cut for PMTs selection criteria");
    declareProperty("opLocation", m_opLocation="DetDesc", "Optical parameters location");
    declareProperty("geomLocation", m_geomLocation="DetDesc", "Optical parameters location");
    declareProperty("AbsLength", m_opPara.m_attl=-1., "Effective absorption length");
    declareProperty("TopReflectivity", m_opPara.m_topRef=-1., "Reflectivity of top reflector");
    declareProperty("BotReflectivity", m_opPara.m_botRef=-1., "Reflectivity of bottom reflector");
    declareProperty("TopRefZ", m_geomPara.m_topRefZ=0., "Z position of top reflector");
    declareProperty("BotRefZ", m_geomPara.m_botRefZ=0., "Z position of bottom reflector");
    declareProperty("m_timeCutLow", m_timeCutLow=-1700., "Lower limit of the TDC cut to get rid of noise");
    declareProperty("m_timeCutHigh", m_timeCutHigh=-1400., "Higher limit of the TDC cut to get rid of noise");
    declareProperty("InitSource", m_src = "/Event/Rec/AdScaled", "The initial value for fitting");
}

QMLFTool::~QMLFTool()
{
}

StatusCode QMLFTool::initialize()
{
    // Get Pmt Geometry Service
    m_pmtGeomSvc = svc<IPmtGeomInfoSvc>(m_pmtGeomSvcName, true);

    // Get Pmt Calib Data Service
    m_pmtCalibDataSvc = svc<ICalibDataSvc>(m_pmtCalibDataSvcName, true);

    // Get Parameters
    getOpPara();
    getGeomPara();
    getPmtEff();

    // Show Optical and Geom Parameters
    printPara();

    return StatusCode::SUCCESS;
}

StatusCode QMLFTool::finalize()
{
    return StatusCode::SUCCESS;
}

StatusCode QMLFTool::reconstruct(const DayaBay::CalibReadout& calibReadout,
	                         DayaBay::RecTrigger& recTrigger)
{
    //tttttttt = 0;
    if ( ! calibReadout.detector().isAD() ) {
	debug() << "Not an AD readout; ignoring detector " 
	        << calibReadout.detector().detName() << endreq;
	recTrigger.setPositionStatus( ReconStatus::kNotProcessed);
	recTrigger.setEnergyStatus( ReconStatus::kNotProcessed);
	return StatusCode::SUCCESS;
    }

    m_calibCrate = dynamic_cast<const DayaBay::CalibReadoutPmtCrate*>(&calibReadout);
    if ( !m_calibCrate ) {
	error() << "Incorrect type of readout crate for detector "
	        << calibReadout.detector().detName() << endreq;
	recTrigger.setPositionStatus(ReconStatus::kBadReadout);
	recTrigger.setEnergyStatus(ReconStatus::kBadReadout);
	return StatusCode::FAILURE;
    }

    // Initial the results from user specified source
    const DayaBay::RecHeader* srcRecHeader = get<DayaBay::RecHeader>(m_src);
    //if ( ! srcRecHeader ) {
    // // error handling
    //}
    const DayaBay::RecTrigger& srcTrigger = srcRecHeader->recTrigger();
    const CLHEP::HepLorentzVector& srcPos = srcTrigger.position();
    double xini = srcPos.x();
    double yini = srcPos.y();
    double zini = srcPos.z();
    double eini = srcTrigger.energy();
    //double tini = recTrigger.triggerTime().GetNanoSec();

    // Set this QMLFTool instance as the current_instance
    current_instance = this;

    // fill the local buffers
    Site::Site_t site = m_calibCrate->detector().site();
    DetectorId::DetectorId_t  detid = m_calibCrate->detector().detectorId();
    m_svcMode = ServiceMode(m_calibCrate->header()->context(), 0);
    if ( site != Site::kFar) {
	m_ADNo = (site>>1)*2 + detid - 1;
    }

for ( int local_id = 0; local_id < 192; ++local_id ) 
{
	m_obsq[local_id] = 0.0;

	int ring = local_id/24 + 1;
	int col  = local_id%24 + 1;
	DayaBay::AdPmtSensor adPmtId(ring, col, site, detid);
	const DayaBay::PmtCalibData* pmtData =
	    m_pmtCalibDataSvc->pmtCalibData(adPmtId, m_svcMode);
	// TODO: optimize : if (m_vPmtData[local_id] == pmtData) break;
	// m_vPmtData[4][192];
	m_vPmtData[local_id] = pmtData;
	if ( pmtData != 0 && pmtData->m_status == DayaBay::PmtCalibData::kGood ) {
	    m_chgLLFUsedPMTs[local_id] = adPmtId.fullPackedData();
	}
	else {
	    m_chgLLFUsedPMTs[local_id] = 0;
	}
}
    DayaBay::CalibReadoutPmtCrate::PmtChannelReadouts::const_iterator chanIter, 
	chanEnd = m_calibCrate->channelReadout().end();
    for(chanIter=m_calibCrate->channelReadout().begin(); chanIter != chanEnd; ++chanIter) 
    {
	    const DayaBay::CalibReadoutPmtChannel& channel = *chanIter;
	    const DayaBay::AdPmtSensor pmtId(channel.pmtSensorId().fullPackedData());
	    if ( ! pmtId.is8inch() ) continue;
	    const std::vector<double>& vTime = channel.time();
    	const std::vector<double>& vChrg = channel.charge();
    	double maxCharge = 0.0;
    	int local_id = (pmtId.ring()-1)*24 + pmtId.column() - 1;
    	std::vector<double>::const_iterator iT = vTime.begin(), iC = vChrg.begin(), iEnd = vChrg.end();
    	while ( iC != iEnd ) 
        {
    	    if ( *iT > m_timeCutLow && *iT < m_timeCutHigh ) 
            {
    		    if ( maxCharge < *iC ) maxCharge = *iC;
	        }
            ++iC; ++iT;
	    }
	    m_obsq[local_id] = maxCharge;
    }
    // End of filling local buffers

    //
    // Set parameters for Minuit minimization
    static const int Npar = 4;
    int    ierflg = 0;
    double standin = 0.0;
    double maxCalls = 2000.;
    double tolerance = 1.0e-3;
    double arglist[2];

    //Create TMinuit
    TMinuit * recMinuit = new TMinuit(Npar);
    recMinuit->SetFCN(QMLFTool::NCLL_FCN);
    recMinuit->SetPrintLevel(-1);

    // Initial TMinuit Fitting Parameter
    recMinuit->mnparm(0, "xpos",   xini,  100.0, 0.0, 0.0, ierflg);
    recMinuit->mnparm(1, "ypos",   yini,  100.0, 0.0, 0.0, ierflg);
    recMinuit->mnparm(2, "zpos",   zini,  10.0, m_geomPara.m_botRefZ, m_geomPara.m_topRefZ, ierflg);
    recMinuit->mnparm(3, "energy", eini,   0.2, 0.001, 1000.0, ierflg);

    // No Warnings
    recMinuit->mnexcm("SET NOW", &standin, 0, ierflg);

    // Set error Definition
    // 1 for Chi square
    // 0.5 for negative log likelihood
    if(m_LLFMode==1)
	recMinuit->SetErrorDef(1);
    else 
	recMinuit->SetErrorDef(0.5);

    // Minimization strategy
    // 1 standard;  2 try to improve minimum (slower)
    arglist[0] = 2;
    recMinuit->mnexcm("SET STR", arglist, 1, ierflg); 

    //Do Minuit fit!  
    arglist[0] = maxCalls;
    arglist[1] = tolerance;
    recMinuit->mnexcm("MIGrad", arglist, 2, ierflg);
    //recMinuit->mnexcm("MINIMIZE", arglist, 2, ierflg);
    //recMinuit->mnexcm("SIMPlex", arglist, 2, ierflg);

    //Scan is usefull to check FCN
    // arglist[0] = 0;
    // recMinuit->mnexcm("SCAN", arglist, 1, ierflg);

    //Collect the output
    //How to draw the n-sigma contour of a Minuit fit?
    //http://root.cern.ch/cgi-bin/print_hit_bold.pl/root/html508/examples/fitcont.C.html?minuit#first_hit

    // record fitter's status
    double ln0, edm, errdef;
    int nvpar, nparx, icstat;
    recMinuit->mnstat(ln0, edm, errdef, nvpar, nparx, icstat);

    // Get Fitted Parameter
    double x_fit, x_fit_err;
    double y_fit, y_fit_err;
    double z_fit, z_fit_err;
    double e_fit, e_fit_err;

    recMinuit->GetParameter(0, x_fit, x_fit_err);
    recMinuit->GetParameter(1, y_fit, y_fit_err);
    recMinuit->GetParameter(2, z_fit, z_fit_err);
    recMinuit->GetParameter(3, e_fit, e_fit_err);

    // error matrix; mnemat()
    double emat[Npar][Npar];
    CLHEP::HepMatrix matrix(Npar, Npar);
    recMinuit->mnemat(&emat[0][0], Npar);
    for(int row=0; row<Npar; row++) {
	for(int col=0; col<Npar; col++) {
	    matrix[row][col] = emat[row][col];
	}
    }

    CLHEP::HepLorentzVector vertex(x_fit, y_fit, z_fit, 0.);
    recTrigger.setPosition(vertex);
    recTrigger.setEnergy(e_fit);
    recTrigger.setPositionQuality(ln0);
    recTrigger.setEnergyQuality(ln0);
    recTrigger.setErrorMatrix(matrix);

    delete recMinuit;
    //std::cout << "ncll_fcn called: " << tttttttt << std::endl;

    return StatusCode::SUCCESS;
}

void QMLFTool::ncll_fcn(int& /*npar*/, double* /*grad*/, double& fval, double* xval, int /*iflag*/)
{
    //++tttttttt;
    // Always calculate the value of the function, FVAL,
    // which is usually a chisquare or log likelihood.

    // Global coefficient (QE, Yield...). Nominal 'absolute' QE: 0.19
    //static const double c_n = 200.; // light speed in liquid, mm/s
    static const double ECAL = m_opPara.m_photoCathodeArea/4.0/TMath::Pi() * m_opPara.m_yield * 0.19;
    static const double fap0 =  0.999946; // 0.9984;
    static const double fap1 =  0.101046; // 0.51547;
    static const double fap2 = -1.04014;  //-2.2329;
    static const double fap3 =  1.01381;  // 2.18043;
    static const double fap4 = -0.410953; //-0.78739;
    //static const double ftp0 = -0.042;
    //static const double ftp1 = 0.11365;
    //static const double ftp2 = 1.0;

    double chg_likelihood = 0.;

    // Loop over calib readout channels
for ( int local_id=0; local_id<192; local_id++ ) 
{
	unsigned int pmtid = m_chgLLFUsedPMTs[local_id];
	if ( pmtid == 0 ) continue;

	// TODO: optimize, buffer the pos and norm locally
	IPmtGeomInfo* geoInfo = m_pmtGeomSvc->get(pmtid);
	const CLHEP::Hep3Vector& pos  = geoInfo->localPosition();
	const CLHEP::Hep3Vector& norm = geoInfo->localDirection();

	const DayaBay::PmtCalibData* pmtData = m_vPmtData[local_id];

	double dx = xval[0] - pos.x();
	double dy = xval[1] - pos.y();
	double dz = xval[2] - pos.z();
	double dist_norm = dx*norm.x() + dy*norm.y();

	double dxy2  = dx*dx + dy*dy;
	double dist2 = dxy2 + dz*dz;
	if ( dist2 < 0.0001 ) dist2 = 0.0001;
	double dist = sqrt(dist2);

	double cosa_tmp = dist_norm / dist;
	if ( cosa_tmp > 1.0 ) cosa_tmp = 1.0;

	double Theta = TMath::ACos(cosa_tmp);
	double S_angle = fap0 + Theta*(fap1 + Theta*(fap2 + Theta*(fap3 + fap4*Theta)));
	if ( S_angle < 1.0e-5 ) S_angle = 1.0e-5;

	double expected_q = S_angle*exp(-dist/m_opPara.m_attl)/dist2;

	// Reflection contribution: NREF_ORDER orders are count
	double dz_up = -dz, dz_down = dz;
	double ref_up = 1.0, ref_down = 1.0;
	double pos2refX2[2] = { 2*(m_geomPara.m_topRefZ - pos.z()),
	                        2*(pos.z() - m_geomPara.m_botRefZ) };
	for ( int rOrder = 0; rOrder < NREF_ORDER; ++rOrder ) 
    {
	    int up_down_flag = (rOrder & 1);  // rOrder % 2

	    // The init direction points up
	    dz_up += pos2refX2[up_down_flag];
	    dist2 = dxy2 + dz_up*dz_up;
	    if ( dist2 < 0.0001 ) dist2 = 0.0001;
	    dist = sqrt(dist2);

	    cosa_tmp = dist_norm / dist;
	    if ( cosa_tmp > 1.0 ) cosa_tmp = 1.0;
	    Theta = TMath::ACos(cosa_tmp);
	    S_angle = fap0 + Theta*(fap1 + Theta*(fap2 + Theta*(fap3 + fap4*Theta)));
	    if ( S_angle < 1.0e-5 ) S_angle = 1.0e-5;

	    ref_up = up_down_flag ? ref_up*m_opPara.m_botRef : ref_up*m_opPara.m_topRef;
	    expected_q += ref_up*S_angle*exp(-dist/m_opPara.m_attl)/dist2;

	    // The init direction points down
	    dz_down += pos2refX2[1-up_down_flag];
	    dist2 = dxy2 + dz_down*dz_down;
	    if ( dist2 < 0.0001 ) dist2 = 0.0001;
	    dist = sqrt(dist2);

	    cosa_tmp = dist_norm / dist;
	    if ( cosa_tmp > 1.0 ) cosa_tmp = 1.0;
	    Theta = TMath::ACos(cosa_tmp);
	    S_angle = fap0 + Theta*(fap1 + Theta*(fap2 + Theta*(fap3 + fap4*Theta)));
	    if ( S_angle < 1.0e-5) S_angle = 1.0e-5;

	    ref_down = up_down_flag ? ref_down*m_opPara.m_topRef : ref_down*m_opPara.m_botRef;
	    expected_q += ref_down*S_angle*exp(-dist/m_opPara.m_attl)/dist2;
	}

	expected_q *= xval[3] * ECAL * m_pmtEff[m_ADNo][local_id];  //pmtData->m_efficiency;

	if ( m_LLFMode == 1 ) {
	    //////////////Likelihood fiting without Charge smearing////////
	    double iCharge = m_obsq[local_id];
	    if ( iCharge != 0 ) 
        {
		    chg_likelihood += 2*((expected_q-iCharge) + log(iCharge/expected_q)*iCharge);
	    }
	    else {
		    chg_likelihood += 2*expected_q;
	    }
	}
	else {
	    /////======================================
	    ////////////////////Likelihood fiting including Charge smearing
	}
} // loop for local_id

    fval = chg_likelihood;
}

void QMLFTool::getOpPara()
{
    if ( m_opLocation == "DetDesc" ) {
	// Get Optical Parameters from Det/DetDesc
	// A tabular was defined in DetDesc, but only one value was get here
	// FIXME
	// Get the Detector Data Service to access TDS:
	// This should be done in your tools initialize()
	// Later, look up some material.  If this material is always used you
	// should look it up once in initialize() and save it for later use

	if ( m_opPara.m_attl < 0 ) { // absorption length
	    m_opPara.m_attl = meanOpticalPara<Material>(
		    "/dd/Materials/GdDopedLS",
		    "ABSLENGTH" );
	}
	if ( m_opPara.m_topRef < 0 ) { // top ESR reflectivity
	    m_opPara.m_topRef = meanOpticalPara<Surface>(
		    "/dd/Geometry/AdDetails/AdSurfacesAll/ESRAirSurfaceTop",
		    "REFLECTIVITY" );
	}
	if ( m_opPara.m_botRef < 0 ) { // bottom ESR reflectivity
	    m_opPara.m_botRef = meanOpticalPara<Surface>(
		    "/dd/Geometry/AdDetails/AdSurfacesAll/ESRAirSurfaceBot",
		    "REFLECTIVITY" );
	}
    }
    else if ( m_opLocation == "OpCalibSvc" ) {
    }
    else {
	warning() <<"No external optical data source, use default value."<< endreq;
    }
}

void QMLFTool::getGeomPara()
{
    if ( m_geomPara.m_topRefZ != 0. && m_geomPara.m_botRefZ != 0. ) return;

    if ( m_geomLocation == "DetDesc" ) {
	IDataProviderSvc* dds = 0;
	StatusCode sc = service( "DetectorDataSvc", dds, true );
	if (sc.isFailure()) {
	    error() << "No DetectorDataSvc available!" << endreq;
	}

	// define a pointer to ICoordSysSvc to
	// use to cache the service during the job.
	ICoordSysSvc* css;
	sc = service( "CoordSysSvc", css, true );

	Gaudi::XYZPoint zero(0,0,0);

	if ( m_geomPara.m_topRefZ == 0. ) {
	    std::string topEsrPath = "/dd/Structure/AdReflectorStructure/db-ad/db-ad1-topesr";
	    SmartDataPtr<IDetectorElement> topEsr(dds,topEsrPath);
	    // Set top ESR position
	    Gaudi::XYZPoint globalPoint = topEsr->geometry()->toGlobal(zero);
	    IDetectorElement* theAdDe = css->coordSysDE(globalPoint);
	    Gaudi::XYZPoint adLocalPoint = theAdDe->geometry()->toLocal(globalPoint);
	    m_geomPara.m_topRefZ = adLocalPoint.z();
	}

	if ( m_geomPara.m_botRefZ == 0. ) {
	    std::string botEsrPath = "/dd/Structure/AdReflectorStructure/db-ad/db-ad1-botesr";
	    SmartDataPtr<IDetectorElement> botEsr(dds,botEsrPath);
	    // Set bot ESR position
	    Gaudi::XYZPoint globalPoint = botEsr->geometry()->toGlobal(zero);
	    IDetectorElement* theAdDe = css->coordSysDE(globalPoint);
	    Gaudi::XYZPoint adLocalPoint = theAdDe->geometry()->toLocal(globalPoint);
	    m_geomPara.m_botRefZ = adLocalPoint.z();
	}
    }
    else if ( m_geomLocation == "OnsiteSuveyData" ) {
	// Get geometry info from onsite suvey data
    }
    else {
	warning() <<"Unknown geometry data source, use default value."<< endreq;
    }
}

void QMLFTool::getPmtEff()
{
    std::string fname = getenv("ADRECROOT");
    fname += "/share/AD-PMT-RQE.txt";
    std::ifstream fs(fname.c_str());

    int tmp;
    for ( int j = 0; j < 192; ++j ) {
	fs >> tmp >> tmp;
	for ( int i = 0; i < 8; ++i ) {
	    fs >> m_pmtEff[i][j];
	}
    }
}

void QMLFTool::printPara()
{
    info() << "Op and Geom parameters used in reconstruction." << endreq;
    info() << "Attenuation length : " << m_opPara.m_attl <<"mm"<< endreq;
    info() << "Top reflectivity : " << m_opPara.m_topRef << endreq;
    info() << "Bottom reflectivity : " << m_opPara.m_botRef << endreq;
    info() << "Top reflector Z pos :" << m_geomPara.m_topRefZ <<"mm"<< endreq;
    info() << "Bot reflector Z pos :" << m_geomPara.m_botRefZ <<"mm"<< endreq;
}

template<typename MType>
double QMLFTool::meanOpticalPara(const std::string& location, const std::string& type)
{
    IDataProviderSvc* dds = 0;
    StatusCode sc = service( "DetectorDataSvc", dds, true );
    if (sc.isFailure()) {
	error() << "No DetectorDataSvc available!" << endreq;
	return -1.;
    }

    MType* element = GaudiCommon<AlgTool>::get<MType>(dds, location);
    typename MType::Tables& element_tab = element->tabulatedProperties();

    typename MType::Tables::const_iterator tbIter = element_tab.begin(); 
    while ( tbIter != element_tab.end() ) {
	if ( (*tbIter)->type() == type ) break;
	++tbIter;
    }
    const TabulatedProperty::Table& par_tab = (*tbIter)->table(); 

    // look up Tables, get an average value for the parameter
    int count = 0;
    double parSum = 0.0;
    for ( TabulatedProperty::Table::const_iterator parIter = par_tab.begin();
	    parIter != par_tab.end();
	    parIter++ )
    {
	if(parIter->first<3.18e-06 && parIter->first>2.07e-06 ) { // 390nm ~ 600nm
	    count++;
	    parSum += parIter->second;
	}
    }

    return parSum/count; // average value between 390nm~600nm
}
