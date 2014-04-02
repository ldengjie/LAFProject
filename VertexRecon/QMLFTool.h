/**
 *  QMLFTool.h
 *
 *  Charge Maximum Likelihood Fitter Tool. 
 *  
 *  The data (MC or Real) showed an OBSERVED charge pattern. 
 *  Based on certain optical model, one can calculate the 
 *  EXPECTED charge pattern for a given source position.
 *  A LIKELIHOOD can be calculated using above two patterns.
 *  Minimize this likelihood will give an estimation of the 
 *  source position.
 *
 *  wenlj@ihep.ac.cn  2008/09/20
 *
**/

#ifndef QMLFTOOL_H
#define QMLFTOOL_H 1

#include "OpPara.h"
#include "GeomPara.h"
#include "Context/ServiceMode.h"
#include "GaudiAlg/GaudiTool.h"
#include "ReconAlg/IReconTool.h"

namespace DayaBay {
    class CalibReadoutPmtCrate;
    class PmtCalibData;
}

class IPmtGeomInfoSvc;
class ICalibDataSvc;

class QMLFTool : public GaudiTool, virtual public IReconTool
{
    private :

	static QMLFTool* current_instance;
	static void NCLL_FCN(int& npar, double* grad, double& fval, double* xval, int iflag);

    public :

	QMLFTool(const std::string& type,
		 const std::string& name,
		 const IInterface* parent);

	virtual ~QMLFTool();

	virtual StatusCode initialize();
	virtual StatusCode finalize();

	virtual StatusCode reconstruct(const DayaBay::CalibReadout&,
		                       DayaBay::RecTrigger&);

    private :

	void ncll_fcn(int& npar, double* grad, double& fval, double* xval, int iflag);

	void getOpPara();
	void getGeomPara();
	void getPmtEff();

	void printPara();

	template<typename MType>
	double meanOpticalPara(const std::string& location, const std::string& type);

    private :

	// different recon mode
	// 1 : not consider charge smearing
	// others : take charge smearing (S.P.E spectrum) into account
	int           m_LLFMode;

	// A flag that whether to add time likelihood or not
	// if false: not to add; if true: add. Default is false.
	bool          m_addTimeLLF;

	// If add time likelihood, the charge cut for PMTs selection criteria
	// It is useful when m_addTimeLLF=1
	double        m_pmtChgCut;

	// Limits of the TDC cut to get rid of noise
	float         m_timeCutLow;
	float         m_timeCutHigh;

	// Pmt Geometry Information Service
	IPmtGeomInfoSvc* m_pmtGeomSvc;
	// Pmt CalibData Information Service
	ICalibDataSvc*   m_pmtCalibDataSvc;

	// Optional response curve, optical and geometry parameters for reconstruction
	OpPara        m_opPara;
	GeomPara      m_geomPara;

	// Local variables
	const DayaBay::CalibReadoutPmtCrate* m_calibCrate;

	const DayaBay::PmtCalibData*  m_vPmtData[192];
	unsigned int  m_chgLLFUsedPMTs[192];
	double        m_obsq[192];
	double        m_pmtEff[8][192];  //TODO: can be optimized
	int           m_ADNo;

	double        m_fixR[192];

	ServiceMode   m_svcMode;

	std::string   m_opLocation;
	std::string   m_geomLocation;
	std::string   m_src;

	// Property PmtGeomSvcName - Name of the PmtGeom service
	std::string   m_pmtGeomSvcName;
	// Property CalibDataSvcName - Name of the CalibData service
	std::string   m_pmtCalibDataSvcName;

	std::string   m_fixf;
};

#endif // QMLFTOOL_H
