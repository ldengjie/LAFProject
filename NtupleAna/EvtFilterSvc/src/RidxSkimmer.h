#ifndef RIDX_SKIMMER_H
#define RIDX_SKIMMER_H

#include "ISkimmer.h"
#include <vector>

class ReconInputSvc;
class TFile;
class TTree;

class RidxSkimmer : public ISkimmer
{
    public :

	RidxSkimmer(const std::string& name);
	virtual ~RidxSkimmer();

	virtual bool write(PhyEvent* evt);

    private :

	void fill();

	ReconInputSvc*     m_svc;

	TFile*             m_file;
	TTree*             m_tree;
	int                m_fNum;
	std::string*       m_fName;
	int                m_nEvt;
	std::vector<int>*  m_idx;
};

#endif
