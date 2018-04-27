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
 *  globals.c
 * 
 * Description:
 *  This file defines global variables.
 *******************************************************************************/



#include "xc.h"
#include "constants.h"
#include "globals.h"
#include "FIR.h"
#include <stdbool.h>
#include "fft.h"

// Buffer for auto notch
int rightAutonotchBuffer[BUFFERLENGTH_DIV_2 + 64];
int * rightAutonotchInput = &rightAutonotchBuffer[0];
int * rightAutonotchDesired = &rightAutonotchBuffer[64];

int leftAutonotchBuffer[BUFFERLENGTH_DIV_2 + 64];
int * leftAutonotchInput = &leftAutonotchBuffer[0];
int * leftAutonotchDesired = &leftAutonotchBuffer[64];

FIRStruct leftFIRfilter,rightFIRfilter,leftFIRfilter2,rightFIRfilter2,rightAutonotchFilter,leftAutonotchFilter;

fractional __attribute__((eds, space(ymemory),aligned)) leftDelayBuffer[NTAPS];
fractional __attribute__((eds, space(ymemory),aligned)) rightDelayBuffer[NTAPS];
fractional __attribute__((eds, space(ymemory),aligned)) leftDelayBuffer2[NTAPS];
fractional __attribute__((eds, space(ymemory),aligned)) rightDelayBuffer2[NTAPS];
fractional __attribute__((eds, space(ymemory),aligned)) leftDelayBuffer3[32];
fractional __attribute__((eds, space(ymemory),aligned)) rightDelayBuffer3[32];


__eds__ int txBufferA[BUFFERLENGTH] __attribute__((space(dma),eds));
__eds__ int txBufferB[BUFFERLENGTH] __attribute__((space(dma),eds));
__eds__ int rxBufferA[BUFFERLENGTH] __attribute__((space(dma),eds));
__eds__ int rxBufferB[BUFFERLENGTH] __attribute__((space(dma),eds));

// DMA flags
volatile bool rxBufferIndicator = 0;

volatile int filterOn = 1, autonotchOn = 0;

unsigned int filter_type = 0;

volatile int fft_data_request = 1;

bool dispFFT = 1;

CONTROLSTATUS volatile controls;

config_struct config = {
	//default values at first startup
	.fft_channel =0,		//0 = left, 1 = right
	.fft_window = 1,		//0 = rectangular, 1 = kaiser, 2 = cheby
	.fft_detect = 0,		//0 = peak, 1 = smooth
	.gain_input = 27,		//27 = +6dB
	.gain_headphone = 111,	//111 = -10dB	
	.contrast_value = 0x0A
};
fractional fft_data[FFT_BLOCK_LENGTH];
buttonStruct topButton, middleButton, bottomButton, leftEncoderButton, rightEncoderButton;
int volatile maxLeftValue, maxRightValue;

int coefficients[NTAPS] __attribute__ ((space(xmemory),far)); 
int coefficients2[NTAPS] __attribute__ ((space(xmemory),far)); 
int coefficients3[NTAPS_NOTCH] __attribute__ ((space(xmemory),far)); 

int iBPFilter, iLPFilter, iHPFilter;
int fc, fLowpass, fHighpass, bandwidth;

int overflowRightFlag = 0;
int overflowLeftFlag = 0;
unsigned int overflowTimer = 0;

int REncValue = 0, LEncValue = 0;

unsigned int REncTimer = 0;
unsigned int LEncTimer = 0;


