#ifndef LAF_ELEMENT_MGR_H
#define LAF_ELEMENT_MGR_H

#include "LafKernel/OptionParser.h"
#include "LafKernel/LafException.h"
#include <map>
#include <list>
#include <vector>
#include <string>

template<typename Element>
class ElementMgr
{
    public :

	const std::string& name() { return m_name; }

	static Element* get(const std::string& objName, bool create = false);

	template<typename ConcreteType>
	static ConcreteType* get(const std::string& objName, bool create = false);

	virtual bool initialize();
	virtual bool finalize();


    private :

	std::string   m_name;


    protected :

	ElementMgr(const std::string& name);
	virtual ~ElementMgr();

	static std::list<Element*>            m_elements;

	static std::map<std::string, Element*>  name2obj;

	typedef Element* (*ElementCreator)(const std::string&);
	typedef std::map<std::string, ElementCreator> Type2CreatorMap;
	static std::map<std::string, ElementCreator>  elementCreatorMap;


    private :

	ElementMgr();
	ElementMgr(const ElementMgr&);
	ElementMgr& operator=(const ElementMgr&);
};

template<typename Element>
std::list<Element*>   ElementMgr<Element>::m_elements;

template<typename Element>
std::map<std::string, Element*>  ElementMgr<Element>::name2obj;

template<typename Element>
typename ElementMgr<Element>::Type2CreatorMap  ElementMgr<Element>::elementCreatorMap;

template<typename Element>
ElementMgr<Element>::ElementMgr(const std::string& name)
    : m_name(name)
{
    std::vector<std::string> vNames;
    OptionParser::setOption(name, "Contents", vNames);

    for(std::vector<std::string>::iterator i=vNames.begin(); i!=vNames.end(); ++i) {
	std::string typName = *i;
	std::string objName = typName;
	std::string::size_type pseg = typName.find('/', 0);
	if ( pseg != std::string::npos ) {
	    typName = typName.substr(0, pseg);
	    objName = objName.substr(pseg+1, std::string::npos);
	}

	typename Type2CreatorMap::iterator j = elementCreatorMap.find(typName);
	if ( j == elementCreatorMap.end() ) {
	    std::string msg = name + ": unknown content type " + typName;
	    throw LafException(msg);
	}
	Element* obj = (j->second)(objName);
	m_elements.push_back(obj);
	name2obj[objName] = obj;
    }
}

template<typename Element>
ElementMgr<Element>::~ElementMgr()
{
    for(typename std::list<Element*>::iterator i=m_elements.begin(); i!=m_elements.end(); ++i) {
	delete (*i);
    }
}

template<typename Element>
Element* ElementMgr<Element>::get(const std::string& objName, bool create)
{
    typename std::map<std::string, Element*>::iterator i = name2obj.find(objName);
    if ( i != name2obj.end() ) {
	return (i->second);
    }
    if ( create ) {
	typename Type2CreatorMap::iterator j = elementCreatorMap.find(objName);
	if ( j != elementCreatorMap.end() ) {
	    Element* obj = (j->second)(objName);
	    m_elements.push_back(obj);
	    name2obj[objName] = obj;
	    return obj;
	}
    }
    return (Element*)0;
}

template<typename Element> template<typename ConcreteType>
ConcreteType* ElementMgr<Element>::get(const std::string& objName, bool create)
{
    return dynamic_cast<ConcreteType*>(get(objName, create));
}

template<typename Element>
bool ElementMgr<Element>::initialize()
{
    for(typename std::list<Element*>::iterator i=m_elements.begin(); i!=m_elements.end(); ++i) {
	if ( ! (*i)->initialize() ) return false;
    }
    return true;
}

template<typename Element>
bool ElementMgr<Element>::finalize()
{
    for(typename std::list<Element*>::iterator i=m_elements.begin(); i!=m_elements.end(); ++i) {
	if ( ! (*i)->finalize() ) return false;
    }
    return true;
}

#endif
