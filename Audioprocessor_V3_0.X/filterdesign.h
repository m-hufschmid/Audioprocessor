#ifndef _FILTERDESIGN_H
#define _FILTERDESIGN_H

enum filterTypes {LOWPASS, HIGHPASS, BANDPASS};

#define N_LP_FILTERS 12
#define N_HP_FILTERS 10
#define N_BP_FILTERS 6

#define FC_MIN 300
#define FC_MAX 3600
#define FC_STEP 50

void build_filter(int filter_type);
int DDS(unsigned int delta);

#endif
