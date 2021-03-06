#ifndef LAF_ALGBASE_H
#define LAF_ALGBASE_H

#include "LafKernel/OptionParser.h"
#include "LafKernel/TOutputSvc.h"
#include "LafKernel/LafLog.h"
#include <string>

class AlgBase
{
  public:

    AlgBase(const std::string& name) : m_name(name) {}
    virtual ~AlgBase(){}

    virtual bool initialize() = 0;
    virtual bool execute() = 0;
    virtual bool finalize() = 0;

    const std::string& name() { return m_name; }

    SvcBase* service(const std::string& name);

    TOutputSvc* ntupleSvc();

    template<typename Type>
    bool setOption(const std::string& option, Type& var);


  protected :

    std::string m_name;

  private :
    //following interfaces are not supported
    AlgBase();
    AlgBase(const AlgBase&);
    AlgBase& operator=(const AlgBase&);
};

template<typename Type>
bool AlgBase::setOption(const std::string& option, Type& var)
{
    return OptionParser::setOption(m_name, option, var);
}

#endif //ALGBASE_H
