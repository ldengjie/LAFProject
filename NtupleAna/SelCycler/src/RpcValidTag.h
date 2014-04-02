#ifndef RPC_VALID_TAG_H
#define RPC_VALID_TAG_H

#include "SelCycler/ITagBase.h"
#include <vector>

class RpcValidTag : public ITagBase
{
    public :

	RpcValidTag(const std::string& name);
	virtual ~RpcValidTag();

	virtual bool tag(PhyEvent* event);

    private :

	bool m_notValid;
    //bool m_veto;
	int  m_fileNum;
	std::vector<std::string>  m_vFlist;
};

#endif
