#ifndef EVT_SKIMMER_H
#define EVT_SKIMMER_H

#include "ISkimmer.h"
#include <map>
#include <vector>

class ReconInputSvc;
class TFile;
class TTree;
class TDirectory;

class EvtSkimmer : public ISkimmer
{
    public :

	EvtSkimmer(const std::string& name);
	virtual ~EvtSkimmer();

	virtual bool write(PhyEvent* evt);

    private :

	bool updateTrees(PhyEvent* evt);
	TDirectory* getDir(const std::string& fullPath);

	typedef std::map<std::string, std::pair<TTree*, TTree*> > TreeMap;

	TFile*           m_file;
	TFile*           m_ifile;
	int              m_fNum;
	TreeMap          m_treeMap;

	ReconInputSvc*   m_svc;
};

#endif
