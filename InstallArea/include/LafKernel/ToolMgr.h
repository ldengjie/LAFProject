#ifndef LAF_TOOL_MGR_H
#define LAF_TOOL_MGR_H

#include "LafKernel/ElementMgr.h"
#include "LafKernel/ToolBase.h"

class ToolMgr : public ElementMgr<ToolBase>
{
    public :

	static ToolMgr* instance();

    private :

	ToolMgr(const std::string& name);

	friend class ToolFactory;
	static ToolMgr* _instance;
};

template<> bool ElementMgr<ToolBase>::initialize();
template<> bool ElementMgr<ToolBase>::finalize();

#endif
