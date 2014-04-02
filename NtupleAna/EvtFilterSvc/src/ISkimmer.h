#ifndef I_SKIMMER_H
#define I_SKIMMER_H

#include <string>

class PhyEvent;

class ISkimmer
{
    public :

	ISkimmer(const std::string& name) : m_name(name) {}
	virtual ~ISkimmer() {}

	virtual bool write(PhyEvent* evt) = 0;

	const std::string& name() { return m_name; }

    private :

	std::string m_name;
};

#endif
