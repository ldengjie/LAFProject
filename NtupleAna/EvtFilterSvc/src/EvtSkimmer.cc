#include "EvtSkimmer.h"
#include "ReconInputSvc/ReconInputSvc.h"
#include "LafKernel/OptionParser.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/SvcMgr.h"
#include "LafKernel/LafLog.h"
#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"

EvtSkimmer::EvtSkimmer(const std::string& name)
    : ISkimmer(name),
      m_file(0),
      m_ifile(0),
      m_fNum(-1)
{
    std::vector<std::string> treeNames;
    OptionParser::setOption(this->name(), "OutputTrees", treeNames);
    if ( treeNames.empty() ) {
	LogWarn << "No OutputTrees in EvtFilterSvc!" << std::endl;
    }
    for ( std::vector<std::string>::iterator it = treeNames.begin(); it != treeNames.end(); ++it ) {
	m_treeMap[*it] = std::make_pair<TTree*, TTree*>(0, 0);
    }

    std::string fname;
    OptionParser::setOption(this->name(), "OutputFile", fname);
    if ( fname.empty() ) {
	throw LafException(this->name() + ".OutputFile is not set properly!");
    }

    const char* preDir = gDirectory->GetPath();
    m_file = new TFile(fname.c_str(), "RECREATE");
    gDirectory->cd(preDir);

    m_svc = SvcMgr::get<ReconInputSvc>("InputSvc");
    if ( m_svc == 0 ) {
	throw LafException(this->name() + ": Failed to get ReconInputSvc!");
    }
}

EvtSkimmer::~EvtSkimmer()
{
    m_file->Write();
    delete m_file;
    delete m_ifile;
}

bool EvtSkimmer::write(PhyEvent* evt)
{
    if ( m_fNum == evt->m_fileNum || updateTrees(evt) ) {
	for ( TreeMap::iterator it = m_treeMap.begin(); it != m_treeMap.end(); ++it ) {
	    it->second.first->GetEntry(evt->m_localEntry);
	    it->second.second->Fill();
	}
	return true;
    }

    return false;
}

bool EvtSkimmer::updateTrees(PhyEvent* evt) {
    m_fNum = evt->m_fileNum;
    if ( m_ifile != 0 ) {
	m_ifile->Close();
	delete m_ifile;
    }

    const char* preDir = gDirectory->GetPath();
    m_ifile = new TFile(m_svc->fname(m_fNum).c_str());
    for ( TreeMap::iterator it = m_treeMap.begin(); it != m_treeMap.end(); ++it ) {
	TTree* tree = (TTree*)m_ifile->Get(it->first.c_str());
	if( tree == 0 ) {
	    LogError << "Can't find tree \"" << it->first << "\" in file " << m_svc->fname(m_fNum) << std::endl;
	    return false;
	}
	if ( it->second.second == 0 ) {  //first input file
	    TDirectory* dir = getDir(it->first);
	    dir->cd();
	    it->second = std::make_pair(tree, tree->CloneTree(0) );
	}
	else {
	    it->second.first = tree;
	    tree->CopyAddresses(it->second.second);
	}
    }
    gDirectory->cd(preDir);

    return true;
}

TDirectory* EvtSkimmer::getDir(const std::string& fullPath)
{
    TDirectory* result = m_file;

    std::list<std::string> vPath;
    std::string::size_type iSeg = 0;
    while ( iSeg != std::string::npos ) {
	std::string::size_type nSeg = fullPath.find('/', iSeg);
	std::string::size_type count = nSeg-iSeg;
	std::string path = fullPath.substr(iSeg, count);
	if ( ! path.empty() ) {
	    vPath.push_back(fullPath.substr(iSeg, count));
	}
	iSeg = (nSeg == std::string::npos) ? nSeg : nSeg + 1;
    }

    vPath.pop_back();

    while ( ! vPath.empty() ) {
	const std::string& path = vPath.front();
	TDirectory* pDir = result->GetDirectory( path.c_str() );
	if ( pDir == 0 ) {
	    pDir = result->mkdir( path.c_str() );
	}
	result = pDir;
	vPath.pop_front();
    }

    return result;
}
