#ifndef LAF_SVC_MGR_H
#define LAF_SVC_MGR_H

#include "LafKernel/ElementMgr.h"
#include "LafKernel/SvcBase.h"

class SvcMgr : public ElementMgr<SvcBase>
{
    public :

	static SvcMgr* instance();


    private :

	SvcMgr(const std::string& name);

	friend class SvcFactory;
	static SvcMgr* _instance;
};

#endif
