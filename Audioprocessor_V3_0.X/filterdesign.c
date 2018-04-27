/*******************************************************************************
 * Project:
 *  "Audioprocessor"
 * 
 * Version / Date:
 *  2.3 / Jan 2018
 * 
 * Authors:
 *  Markus Hufschmid
 * 
 * File Name:
 *  filterdesign.c
 * 
 * Description:
 *  Functions to compute the impulse responses of the filter
 *******************************************************************************/

#include "filterdesign.h"
#include "init.h"
#include "globals.h"
#include "FIR.h"
#include "SineTable.h"
#include <dsp.h>

#include "LPcoefficients.h"
#include "HPcoefficients.h"
#include "BPcoefficients.h"

const unsigned int LP_FREQUENCIES[N_LP_FILTERS] = {1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200};
const unsigned int HP_FREQUENCIES[N_HP_FILTERS] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
const unsigned int BP_BANDWIDTHS[N_BP_FILTERS] = {100, 200, 300, 400, 500, 600};

const int *LP_PTRS[N_LP_FILTERS] = {&LP1000Hz[0], &LP1400Hz[0], &LP1400Hz[0], &LP1600Hz[0], &LP1800Hz[0], &LP2000Hz[0], &LP2200Hz[0], &LP2400Hz[0], &LP2600Hz[0], &LP2800Hz[0], &LP3000Hz[0], &LP3200Hz[0]};
const int *HP_PTRS[N_HP_FILTERS] = {&HP100Hz[0], &HP200Hz[0], &HP300Hz[0], &HP400Hz[0], &HP500Hz[0], &HP600Hz[0], &HP700Hz[0], &HP800Hz[0], &HP900Hz[0], &HP1000Hz[0]};
const int *BP_PTRS[N_BP_FILTERS] = {&LP50Hz[0], &LP100Hz[0], &LP150Hz[0], &LP200Hz[0], &LP250Hz[0], &LP300Hz[0]};

extern int coefficients[];

void build_filter(int filter_type){
	int i;
	const int *coeffPtr;
	long int temp;
	unsigned int delta, phase;

	switch (filter_type){
		case BANDPASS:
			bandwidth = BP_BANDWIDTHS[iBPFilter];
			coeffPtr = BP_PTRS[iBPFilter];
				delta = 65536UL*fc/8000UL;
				phase = 0;	
				for (i=0; i<101; i++){
					temp = SINE_TABLE[((phase >> 4) + 1024) & 4095];
					phase += delta;
					temp = temp * *(coeffPtr + i);
					temp = temp >> 14;
					coefficients[i+100] = (int) temp;
					coefficients[100-i] = (int) temp;
				}
			break;
		case LOWPASS:
			fLowpass = LP_FREQUENCIES[iLPFilter];
			coeffPtr = LP_PTRS[iLPFilter];
				for (i=0; i<101; i++){
					coefficients[i+100] = *(coeffPtr + i);
					coefficients[100-i] = *(coeffPtr + i);
				}
	
			break;
		case HIGHPASS:
			fHighpass = HP_FREQUENCIES[iHPFilter];
			coeffPtr = HP_PTRS[iHPFilter];
				for (i=0; i<101; i++){
					coefficients2[i+100] = *(coeffPtr + i);
					coefficients2[100-i] = *(coeffPtr + i);
				}
	
			break;
	}
}

int DDS(unsigned int delta){
	static unsigned int phase = 0;

	phase += delta;
	return (SINE_TABLE[phase >> 4]);
}



