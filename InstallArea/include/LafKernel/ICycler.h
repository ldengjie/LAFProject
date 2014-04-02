#ifndef LAF_I_CYCLER_H
#define LAF_I_CYCLER_H

class EvtObject;

class ICycler
{
    public :
	virtual ~ICycler() {}
	virtual EvtObject* next(int nth = 1) = 0;
};

#endif
