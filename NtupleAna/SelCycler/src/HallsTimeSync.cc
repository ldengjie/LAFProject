#include "HallsTimeSync.h"
#include "LafKernel/OptionParser.h"
#include "LafKernel/PhyEvent.h"
#include "LafKernel/LafException.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

HallsTimeSync::HallsTimeSync()
{
    std::string fSync;
    OptionParser::setOption("HallsTimeSync", "SyncInput", fSync);

    if ( access( fSync.c_str(), F_OK ) < 0 ) {
	std::string msg = std::string("Invalid HallsTimeSync.SyncInput: ") + fSync;
	throw LafException(msg);
    }

    int pStart_s, pStart_ns, pEnd_s, pEnd_ns;
    std::ifstream fs(fSync.c_str());
    fs >> pStart_s >> pStart_ns >> pEnd_s >> pEnd_ns;

    while ( fs.good() ) {
	m_periods.push_back(std::make_pair(TTimeStamp(pStart_s, pStart_ns),
		                           TTimeStamp(pEnd_s, pEnd_ns)));
	fs >> pStart_s >> pStart_ns >> pEnd_s >> pEnd_ns;
    }

    m_periods.push_back(std::make_pair(TTimeStamp(0x7FFFFFFF, 0),
		                       TTimeStamp(0x7FFFFFFF, 0)));
    m_pEnd = m_periods.front().second;
}

bool HallsTimeSync::validate(PhyEvent* evt)
{
    while ( evt->m_trigTime > m_pEnd ) {
	m_periods.pop_front();
	m_pEnd = m_periods.front().second;
    }

    return evt->m_trigTime >= m_periods.front().first;
}
