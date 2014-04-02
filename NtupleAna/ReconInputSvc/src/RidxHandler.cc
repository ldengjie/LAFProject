#include "RidxHandler.h"
#include "TFile.h"

RidxHandler::RidxHandler(const std::vector<std::string>& ridxs)
    : m_fcount(-1),
      m_count(-1),
      m_max(0),
      m_entry(0)
{
    m_chain = new TChain("EvtIdx", "EvtIdx");
    for ( std::vector<std::string>::const_iterator it = ridxs.begin(); it != ridxs.end(); ++it ) {
	m_chain->AddFile(it->c_str());
    }
    std::string*       fname = 0;
    int                nEvt;
    m_chain->SetBranchStatus("entry", 0);
    m_chain->SetBranchAddress("fname", &fname);
    m_chain->SetBranchAddress("nEvt",  &nEvt);

    Long64_t nfiles = m_chain->GetEntries();
    m_offset = new Long64_t[nfiles+1];
    m_offset[0] = 0;

    for ( Long64_t i = 0; i < nfiles; ++i ) {
	m_chain->GetEntry(i);
	m_max += nEvt;
	m_offset[i+1] = m_max;
	m_roots.push_back(*fname);
    }

    m_chain->SetBranchStatus("*",     0);
    m_chain->SetBranchStatus("entry", 1);
    m_chain->SetBranchAddress("entry", &m_entry);
}

RidxHandler::~RidxHandler()
{
    delete m_chain->GetCurrentFile();
    delete [] m_offset;
}

bool RidxHandler::seek(int n)
{
    m_count = n;
    if ( m_count >= m_max ) return false;

    bool nextfile = false;
    while ( m_count >= m_offset[m_fcount+1] ) {
	++m_fcount;
	nextfile = true;
    }

    if ( nextfile ) {
	m_chain->GetEntry(m_fcount);
    }

    return true;
}

Int_t  RidxHandler::fIndex()
{
    return m_fcount;
}

Long64_t  RidxHandler::eIndex()
{
    return m_entry->at( m_count - m_offset[m_fcount] );
}
