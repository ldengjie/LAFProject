#ifndef I_PER_DATA_SEC_H
#define I_PER_DATA_SEC_H

class IPerDataSec
{
    public :

	virtual ~IPerDataSec() {}

	virtual DataSec* ToTran(long entry) = 0;
};

#endif