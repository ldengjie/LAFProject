#ifndef LAF_ALG_MGR_H
#define LAF_ALG_MGR_H

#include "LafKernel/ElementMgr.h"
#include "LafKernel/AlgBase.h"

class AlgMgr : public ElementMgr<AlgBase>
{
    public :

	static AlgMgr* instance();

	bool execute();


    private :

	AlgMgr(const std::string& name);

	friend class AlgFactory;
	static AlgMgr* _instance;
};

#endif
