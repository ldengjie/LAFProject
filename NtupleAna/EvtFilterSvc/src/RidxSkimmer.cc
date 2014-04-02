#include "RidxSkimmer.h"
#include "ReconInputSvc/ReconInputSvc.h"
#include "LafKernel/OptionParser.h"
#include "LafKernel/SvcMgr.h"
#include "LafKernel/PhyEvent.h"
#include "TFile.h"
#include "TTree.h"

RidxSkimmer::RidxSkimmer(const std::string& name)
    : ISkimmer(name),
      m_file(0),
      m_tree(0),
      m_fNum(-1),
      m_fName(0),
      m_idx(0)
{
    std::string fname;
    OptionParser::setOption(this->name(), "OutputFile", fname);
    if ( fname.empty() ) {
	throw LafException(this->name() + ".OutputFile is not set properly!");
    }

    const char* preDir = gDirectory->GetPath();
    m_file = new TFile(fname.c_str(), "RECREATE");
    m_tree = new TTree("EvtIdx", "index of filtered events");
    m_tree->Branch("fname", &m_fName);
    m_tree->Branch("nEvt",  &m_nEvt);
    m_tree->Branch("entry", &m_idx);
    gDirectory->cd(preDir);

    m_svc = SvcMgr::get<ReconInputSvc>("InputSvc");
    if ( m_svc == 0 ) {
	throw LafException(this->name() + ": Failed to get ReconInputSvc!");
    }
}

RidxSkimmer::~RidxSkimmer()
{
    fill();
    m_file->Write();
    delete m_file;
}

bool RidxSkimmer::write(PhyEvent* evt)
{
    if ( evt->m_fileNum != m_fNum ) {
	fill();
	m_fNum = evt->m_fileNum;
	*m_fName = m_svc->fname(m_fNum);
	m_idx->clear();
    }

    //LogDebug << *m_fName << ": " << evt->m_localEntry << std::endl;
    m_idx->push_back(evt->m_localEntry);

    return true;
}

void RidxSkimmer::fill()
{
    if ( m_fNum >= 0 ) {
	m_nEvt = m_idx->size();
	m_tree->Fill();
    }
}
