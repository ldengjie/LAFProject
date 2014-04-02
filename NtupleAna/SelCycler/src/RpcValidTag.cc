#include "RpcValidTag.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/ToolFactory.h"
#include <algorithm>
#include <fstream>

DECLARE_TOOL(RpcValidTag);

RpcValidTag::RpcValidTag(const std::string& name)
    : ITagBase(name),
      m_notValid(false),
      m_fileNum(-1)
{
    //std::string  listname;
    std::vector<std::string> listname;
    setOption("GoodFileList", listname);
    //setOption("ApplyVeto", m_veto=false);

    std::string first = listname[0];
    //std::cout<<"first of listname: "<<first<<std::endl;
    std::string  fname;
    std::ifstream  fs(first.c_str());
    //while ( (fs>>fname).good() ) {
    while ( fs>>fname ) {
        int fpos=fname.rfind("_");
        std::string fnameNum=fname.substr(fpos);
	    m_vFlist.push_back(fnameNum);
    }
}

RpcValidTag::~RpcValidTag()
{
}

bool RpcValidTag::tag(PhyEvent* event)
{
    if ( event->m_det != 7 )  return false;  //not RPC event

    if ( m_fileNum != event->m_fileNum ) {
	    m_fileNum = event->m_fileNum;
        std::string eventFilename=event->getFileName();
        int eventfpos=eventFilename.rfind("_");
        std::string eventFilenameNum=eventFilename.substr(eventfpos);
	    //std::vector<std::string>::iterator itf = std::find(m_vFlist.begin(), m_vFlist.end(), event->getFileName());
	    std::vector<std::string>::iterator itf = std::find(m_vFlist.begin(), m_vFlist.end(), eventFilenameNum);

	    m_notValid = (itf == m_vFlist.end());
        /*
        std::cout<<"nihao "<<std::endl;
        std::cout<<"m_vFlist size  : "<<m_vFlist.size()<<std::endl;
        for( std::vector<std::string>::iterator it=m_vFlist.begin() ; it!=m_vFlist.end() ; ++it )
        {
            std::cout<<"m_vFlist  : "<<*it<<std::endl;
        }
        
        std::cout<<"eventFilenameNum  : "<<eventFilenameNum<<std::endl;
        std::cout<<"m_notValid  : "<<m_notValid<<std::endl;
        */
    }
    return m_notValid;
}
