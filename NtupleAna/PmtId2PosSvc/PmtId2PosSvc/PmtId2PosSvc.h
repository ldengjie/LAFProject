#ifndef PMTID_2_POS_SVC_H
#define PMTID_2_POS_SVC_H

#include "LafKernel/SvcBase.h"
#include "TVector3.h"

class PhyEvent;

class PmtId2PosSvc : public SvcBase
{
    public :

	PmtId2PosSvc(const std::string& name);
	virtual ~PmtId2PosSvc();

	bool initialize();
	bool finalize();

	const TVector3& hitPosition(PhyEvent* evt, int index);
	const std::vector<TVector3> hitPositions(PhyEvent* evt);


    private :

	void init(PhyEvent* evt);

	bool m_notReady;

	TVector3     m_hits[2][9][32][2];
	std::string  m_ifile;
};

#endif
