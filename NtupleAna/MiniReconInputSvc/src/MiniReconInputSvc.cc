#include "MiniReconInputSvc.h"
#include "RidxHandler.h"
#include "PerEvent/PerPhyEvent.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/SvcFactory.h"
#include <fstream>

DECLARE_SERVICE(MiniReconInputSvc);

MiniReconInputSvc::MiniReconInputSvc(const std::string& name)
    : SvcBase(name),
      m_entry(-1),
      m_evt(0),
      m_ridxHandler(0)
{
    m_name = "MiniReconInputSvc";  //to avoid naming confusion
    setOption("InputFiles",       m_files);
    setOption("PreferredRecTree", m_preferredRecTree = "Event/Rec/AdScaled");
    setOption("PreferredEnergy",  m_preferredEnergy  = "enrec");
    parse_files();
}

MiniReconInputSvc::~MiniReconInputSvc()
{
}

bool MiniReconInputSvc::initialize()
{
    m_evt = new PerPhyEvent(m_files, m_preferredRecTree, m_preferredEnergy);
    PhyEvent::m_inputFlist = m_files;
    return true;
}

bool MiniReconInputSvc::finalize()
{
    delete m_evt;
    if ( m_ridxHandler != 0 ) delete m_ridxHandler;
    return true;
}

EvtObject* MiniReconInputSvc::next(int nth)
{
    PhyEvent* res = 0;
    m_entry += nth;
    if ( m_ridxHandler == 0 ) {
	res = m_evt->makePhyEvent(m_entry);
    }
    else {
	if ( m_ridxHandler->seek(m_entry) ) {
	    res = m_evt->makePhyEvent(m_ridxHandler->fIndex(), m_ridxHandler->eIndex());
	}
    }
    return res;
}

const std::string& MiniReconInputSvc::fname(int num)
{
    return m_files.at(num);
}

void MiniReconInputSvc::parse_files()
{
    std::string first = m_files[0];
    if ( first.substr(first.length()-5) == ".list" ) {
	std::vector<std::string> infiles;
	for(std::vector<std::string>::iterator it=m_files.begin(); it!=m_files.end(); ++it){
	    std::ifstream  fs((*it).c_str());
	    std::string fname;
	    while ( (fs>>fname).good() ) {
		//std::cout << fname << std::endl;
		infiles.push_back(fname);
	    }
	}
	m_files = infiles;
    }

    first = m_files[0];
    if ( first.substr(first.length()-5) == ".ridx" ) {
	m_ridxHandler = new RidxHandler(m_files);
	m_files = m_ridxHandler->rootFiles();
    }

    first = m_files[0];
    if ( first.substr(first.length()-5) != ".root" ) {
	LogFatal << "Only .list/ridx/root input files are permitted !" << std::endl;
    }
}
