#include "LafKernel/TOutputSvc.h"
#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include <iostream>
#include <list>

TOutputSvc::TOutputSvc(const std::string& name)
    : SvcBase(name)
{
}

TOutputSvc::~TOutputSvc()
{
}

bool TOutputSvc::initialize()
{
    std::map<std::string, std::string> fmap;
    OptionParser::setOption(name(), "Output", fmap);

    for ( std::map<std::string, std::string>::iterator it = fmap.begin(); it != fmap.end(); ++it ) {
	addFile(it->first, it->second);
    }

    return true;
}

bool TOutputSvc::finalize()
{
    std::map<std::string, TFile*>::iterator it = m_key2file.begin();
    while ( it != m_key2file.end() ) {
	it->second->Write();
	delete it->second;
	++it;
    }
    m_key2file.clear();
    return true;
}

TTree* TOutputSvc::bookTree(const std::string& fullPath, const std::string& title)
{
    std::string::size_type lSeg = fullPath.rfind('/');
    std::string name = fullPath.substr(lSeg+1, std::string::npos);
    std::string fullDirs = fullPath.substr(0, lSeg);

    TTree* result = new TTree(name.c_str(), title.c_str());
    attach(fullDirs, result);
    return result;
}

TNtuple* TOutputSvc::bookNtuple(const std::string& fullPath, const std::string& title, const std::string& leafs)
{
    std::string::size_type lSeg = fullPath.rfind('/');
    std::string name = fullPath.substr(lSeg+1, std::string::npos);
    std::string fullDirs = fullPath.substr(0, lSeg);

    TNtuple* result = new TNtuple(name.c_str(), title.c_str(), leafs.c_str());
    attach(fullDirs, result);
    return result;
}

bool TOutputSvc::addFile(const std::string& fKey, const std::string& fname)
{
    std::map<std::string, TFile*>::iterator it = m_key2file.find(fKey);
    if ( it == m_key2file.end() ) {
	const char* preDir = gDirectory->GetPath();
	m_key2file[fKey] = new TFile(fname.c_str(), "RECREATE");
	gDirectory->cd(preDir);
	return true;
    }
    return false;
}

TDirectory* TOutputSvc::getDir(const std::string& fullPath)
{
    TDirectory* result = 0;

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

    if ( vPath.empty() ) vPath.push_back(fullPath);

    const std::string& fKey = vPath.front();
    std::map<std::string, TFile*>::iterator it = m_key2file.find(fKey);
    if ( it == m_key2file.end() ) {
	return result;
    }
    result = it->second;

    vPath.pop_front();
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
