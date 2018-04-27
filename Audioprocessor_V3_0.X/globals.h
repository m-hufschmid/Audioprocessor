#ifndef _GLOBALS_H
#define	_GLOBALS_H

#include <dsp.h>
#include <stdbool.h>
#include "constants.h"
#include "FIR.h"

// Buffer for auto notch
extern int rightAutonotchBuffer[BUFFERLENGTH_DIV_2 + 64];
extern int * rightAutonotchInput;
extern int * rightAutonotchDesired;

extern int leftAutonotchBuffer[BUFFERLENGTH_DIV_2 + 64];
extern int * leftAutonotchInput;
extern int * leftAutonotchDesired;

extern FIRStruct leftFIRfilter,rightFIRfilter,leftFIRfilter2,rightFIRfilter2,rightAutonotchFilter,leftAutonotchFilter;

extern fractional __attribute__((eds, space(ymemory),aligned)) leftDelayBuffer[NTAPS];
extern fractional __attribute__((eds, space(ymemory),aligned)) rightDelayBuffer[NTAPS];
extern fractional __attribute__((eds, space(ymemory),aligned)) leftDelayBuffer2[NTAPS];
extern fractional __attribute__((eds, space(ymemory),aligned)) rightDelayBuffer2[NTAPS];
extern fractional __attribute__((eds, space(ymemory),aligned)) leftDelayBuffer3[32];
extern fractional __attribute__((eds, space(ymemory),aligned)) rightDelayBuffer3[32];


extern __eds__ int txBufferA[BUFFERLENGTH] __attribute__((space(dma),eds));
extern __eds__ int txBufferB[BUFFERLENGTH] __attribute__((space(dma),eds));
extern __eds__ int rxBufferA[BUFFERLENGTH] __attribute__((space(dma),eds));
extern __eds__ int rxBufferB[BUFFERLENGTH] __attribute__((space(dma),eds));

extern volatile bool rxBufferIndicator;

extern volatile int filterOn;

extern volatile int autonotchOn;

extern unsigned int filter_type;

extern volatile int fft_data_request;


extern bool dispFFT;


typedef struct {
	int fft_channel;
	int fft_window;
	int fft_detect;
	int gain_input;
	int gain_headphone;
	int contrast_value;
}  config_struct;

typedef struct {
	int state;
	int timer;
	int counter;
} buttonStruct;

typedef struct _CONTROLFLAGS {
	unsigned REnc:1;
	unsigned LEnc:1;
	unsigned REncButton:1;
	unsigned LEncButton:1;
	unsigned TopButton:1;
	unsigned MiddleButton:1;
	unsigned BottomButton:1;
	unsigned anyButton:1;
} CONTROLFLAGS;

typedef union _CONTROLSTATUS {
	int status;
	CONTROLFLAGS flags;
} CONTROLSTATUS;

extern volatile CONTROLSTATUS controls;
extern config_struct config;
extern fractional fft_data[];
extern buttonStruct topButton, middleButton, bottomButton, leftEncoderButton, rightEncoderButton;
extern volatile int maxLeftValue, maxRightValue;

#define NTAPS 201
#define NTAPS_NOTCH 32

// FIR Delay Buffers

/*
extern int leftDelayBuffer[NTAPS] __attribute__ ((space(ymemory),far)); 
extern int rightDelayBuffer[NTAPS]; 
extern int leftDelayBuffer2[NTAPS]; 
extern int rightDelayBuffer2[NTAPS]; 
extern int leftDelayBuffer3[32]; 
extern int rightDelayBuffer3[32]; 
*/
/*
extern __eds__ int leftDelayBuffer[NTAPS] __attribute__((space(ymemory),eds, aligned)); 
extern __eds__ int rightDelayBuffer[NTAPS] __attribute__((space(ymemory),eds, aligned)); 
extern __eds__  int leftDelayBuffer2[NTAPS] __attribute__((space(ymemory),eds, aligned)); 
extern __eds__ int rightDelayBuffer2[NTAPS] __attribute__((space(ymemory),eds, aligned)); 
extern __eds__ int leftDelayBuffer3[NTAPS_NOTCH] __attribute__((space(ymemory),eds, aligned)); 
extern __eds__ int rightDelayBuffer3[NTAPS_NOTCH] __attribute__((space(ymemory),eds, aligned)); 
*/

// Filter Coefficients

extern int coefficients[NTAPS] __attribute__ ((space(xmemory),far)); 
extern int coefficients2[NTAPS] __attribute__ ((space(xmemory),far)); 
extern int coefficients3[NTAPS_NOTCH] __attribute__ ((space(xmemory),far)); 


//Information about filters
extern int iBPFilter, iLPFilter, iHPFilter;
extern int fc, fLowpass, fHighpass, bandwidth;


extern int overflowRightFlag;
extern int overflowLeftFlag;
extern unsigned int overflowTimer;

extern int REncValue, LEncValue;
extern unsigned int REncTimer;
extern unsigned int LEncTimer;

#define OVERFLOW_LIMIT	29205		//corresponds to -1 dB


#endif	/* _GLOBALS_H */

