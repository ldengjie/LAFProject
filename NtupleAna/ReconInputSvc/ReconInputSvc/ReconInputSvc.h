#ifndef RECON_INPUT_SVC_H
#define RECON_INPUT_SVC_H

#include "LafKernel/SvcBase.h"
#include "LafKernel/ICycler.h"

class EvtObject;
class PerPhyEvent;
class RidxHandler;

class ReconInputSvc : public SvcBase,
                    virtual public ICycler
{
    public :

	ReconInputSvc(const std::string& name);
	virtual ~ReconInputSvc();

	virtual bool initialize();
	virtual bool finalize();

	virtual EvtObject* next(int nth = 1);

	const std::string& fname(int num);


    private :

	void parse_files();

	long          m_entry;
	PerPhyEvent*  m_evt;

	std::vector<std::string>  m_files;
	std::string               m_preferredRecTree;
	std::string               m_preferredEnergy;

	RidxHandler*  m_ridxHandler;
};

#endif
