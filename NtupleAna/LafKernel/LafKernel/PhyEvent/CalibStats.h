#ifndef CALIB_STATS_H
#define CALIB_STATS_H

#include "LafKernel/PhyEvent/DataSec.h"

class CalibStats : public DataSec {

    public :

	// Declaration of leaf types
	float         ColumnKurtosis;
	float         EarlyCharge;
	float         Kurtosis;
	float         LateCharge;
	float         MainPeakRMS;
	//move to PhyEvent... float         MiddleTimeRMS;
	float         NominalCharge;
	float         PeakRMS;
	float         Q1;
	float         Q2;
	float         Q3;
	float         QuadrantQ1;
	float         QuadrantQ2;
	float         QuadrantQ3;
	float         QuadrantQ4;
	float         RingKurtosis;
	float         SecondPeakRMS;
	float         charge_sum_flasher_max;
	int           flasher_column;
	float         flasher_flag;
	int           flasher_ring;
	float         tRMS;
	float         time_PSD;
	float         time_PSD1;
	float         time_PSD_local_RMS;

	int           MaxQRing;  //to be added
	int           MaxQCol;   //to be added
};

#endif
