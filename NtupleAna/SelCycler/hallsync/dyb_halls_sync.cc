#include "TTimeStamp.h"
#include <list>
#include <utility>
#include <iostream>
#include <fstream>

using namespace std;

typedef list<pair<TTimeStamp, TTimeStamp> > Periods;

Periods* read_one_hall(const char* fname)
{
    Periods* periods = new Periods;

    int pStart_s, pStart_ns, pEnd_s, pEnd_ns;
    float pLast;
    ifstream ifs(fname);
    ifs >> pStart_s >> pStart_ns >> pEnd_s >> pEnd_ns >> pLast;

    while ( ifs.good() ) {
	//cout << pStart << "  " << pEnd << "  " << pLast << endl;
	periods->push_back(make_pair(TTimeStamp(pStart_s, pStart_ns),
		                     TTimeStamp(pEnd_s, pEnd_ns)));
	ifs >> pStart_s >> pStart_ns >> pEnd_s >> pEnd_ns >> pLast;
    }

    return periods;
}

TTimeStamp period_start(Periods* hall[])
{
    for ( int i = 0; i < 3; ++i ) {
	if ( hall[i]->empty() ) return TTimeStamp(-1, 0);
    }

    for ( int i = 1; i < 3; ++i ) {
	if ( hall[i]->front().first > hall[0]->front().first ) {
	    Periods* tmp = hall[0];
	    hall[0] = hall[i];
	    hall[i] = tmp;
	}
    }

    for ( int i = 1; i < 3; ++i ) {
	while ( hall[i]->front().second <= hall[0]->front().first ) {
	    hall[i]->pop_front();
	    return period_start(hall);
	}
    }

    return hall[0]->front().first;
}

TTimeStamp period_end(Periods* hall[])
{
    for ( int i = 1; i < 3; ++i ) {
	if ( hall[i]->front().second < hall[0]->front().second ) {
	    Periods* tmp = hall[0];
	    hall[0] = hall[i];
	    hall[i] = tmp;
	}
    }

    TTimeStamp pend = hall[0]->front().second;
    hall[0]->pop_front();

    return pend;
}

int main(void)
{
    Periods* hall[3] = {
	read_one_hall("DaqTime_eh1.txt"),
	read_one_hall("DaqTime_eh2.txt"),
	read_one_hall("DaqTime_eh3.txt")
    };

    ofstream fsync("DaqTime_dyb.txt");

    TTimeStamp pStart = period_start(hall);
    while ( pStart >= TTimeStamp(0, 0) ) {
	TTimeStamp pEnd = period_end(hall);
	fsync << pStart.GetSec() << '\t' << pStart.GetNanoSec()
	      << '\t' << pEnd.GetSec() << '\t' << pEnd.GetNanoSec()
	      << endl;
	pStart = period_start(hall);
    }

    for ( int i = 0; i < 3; ++i ) delete hall[i];

    return 0;
}
