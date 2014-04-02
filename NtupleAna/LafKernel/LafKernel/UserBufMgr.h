#ifndef LAF_USER_BUF_MGR_H
#define LAF_USER_BUF_MGR_H

#include "LafKernel/ElementMgr.h"
#include "LafKernel/UserBuffer.h"

class UserBufMgr : protected ElementMgr<UserBuffer>
{
    public :

	static UserBufMgr* instance();
	static void   release();

	static PhyEventBuf* bufWithSelector(const std::string& selector);

	void tryPushBack(PhyEvent* evt);
	void synchronize(PhyEventBuf* buf);


    private :

	UserBufMgr(const std::string& name);

	friend class UserBufFactory;
	static UserBufMgr* _instance;
};

template<> ElementMgr<UserBuffer>::ElementMgr(const std::string& name);
template<> bool ElementMgr<UserBuffer>::initialize();
template<> bool ElementMgr<UserBuffer>::finalize();

#endif
