#ifndef FFT_H
#define FFT_H

#include "GenericTypeDefs.h"

#define FFT_BLOCK_LENGTH 128
#define LOG2N 7


void computeFFT(int *data);
void plotFFT(void);
UINT16 dirtyLog(UINT16 x);
UINT16 intSqrt(UINT32 x);

#endif

