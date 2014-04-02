#ifndef LAF_RIDX_HANDLER_H
#define LAF_RIDX_HANDLER_H

#include "TChain.h"

class RidxHandler
{
    public :

	RidxHandler(const std::vector<std::string>& ridxs);
	virtual ~RidxHandler();

	bool      seek(int n);
	Int_t     fIndex();
	Long64_t  eIndex();

	const std::vector<std::string>& rootFiles() { return m_roots; }


    private :

	Int_t        m_fcount;
	Long64_t     m_count;
	Long64_t     m_max;
	Long64_t*    m_offset;

	TChain*                   m_chain;
	std::vector<int>*         m_entry;
	std::vector<std::string>  m_roots;

	RidxHandler();
};

#endif
