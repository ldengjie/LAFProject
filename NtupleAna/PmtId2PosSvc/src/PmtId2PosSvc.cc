#include "PmtId2PosSvc/PmtId2PosSvc.h"
#include "LafKernel/PhyEvent/CalibReadout.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/SvcFactory.h"
#include <fstream>

DECLARE_SERVICE(PmtId2PosSvc);

PmtId2PosSvc::PmtId2PosSvc(const std::string& name)
    : SvcBase(name),
      m_notReady(true)
{
    setOption("DataFile", m_ifile);
}

PmtId2PosSvc::~PmtId2PosSvc()
{
}

bool PmtId2PosSvc::initialize()
{
    return true;
}

bool PmtId2PosSvc::finalize()
{
    return true;
}

const TVector3& PmtId2PosSvc::hitPosition(PhyEvent* evt, int index)
{
    if ( m_notReady ) init(evt);

    CalibReadout* calib = evt->get<CalibReadout>("Event/CalibReadout/CalibReadoutHeader");

    return m_hits[evt->m_det-5][calib->wallNumber[index]-1][calib->wallSpot[index]-1][calib->inwardFacing[index]];
}

const std::vector<TVector3> PmtId2PosSvc::hitPositions(PhyEvent* evt)
{
    if ( m_notReady ) init(evt);

    std::vector<TVector3> vHits;
    CalibReadout* calib = evt->get<CalibReadout>("Event/CalibReadout/CalibReadoutHeader");

    for ( int i = 0; i < calib->nHitsPool; ++i ) {
	vHits.push_back(m_hits[evt->m_det-5][calib->wallNumber[i]-1][calib->wallSpot[i]-1][calib->inwardFacing[i]]);
    }

    return vHits;
}

void PmtId2PosSvc::init(PhyEvent* evt)
{
    std::ifstream fs(m_ifile.c_str());

    /*
     * det:          5, 6
     * wallNumber:   1 - 9
     * wallSpot:     1 - 24, 1 - 32
     * inwardFacing: 0, 1
     */
    unsigned int index, det, wallNumber, wallSpot, inwardFacing;
    double x, y, z;

    if ( evt->m_site == 4 ) {  // far site
	char tmp[200];
	for ( int i = 0; i < 295; ++i ) fs.getline(tmp, 200);

	for ( int i = 0; i < 384; ++i ) {
	    fs >> index >> det >> wallNumber >> wallSpot >> inwardFacing >> x >> y >> z;
	    m_hits[det-5][wallNumber-1][wallSpot-1][inwardFacing].SetXYZ(x, y, z);
	}
    }
    else {  // near sites
	char tmp[200];
	for ( int i = 0; i < 3; ++i ) fs.getline(tmp, 200);

	for ( int i = 0; i < 288; ++i ) {
	    fs >> index >> det >> wallNumber >> wallSpot >> inwardFacing >> x >> y >> z;
	    m_hits[det-5][wallNumber-1][wallSpot-1][inwardFacing].SetXYZ(x, y, z);
	}
    }

    m_notReady = false;
}
