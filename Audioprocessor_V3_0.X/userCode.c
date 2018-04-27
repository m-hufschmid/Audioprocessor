/*******************************************************************************
 * Project:
 *  "Audioprocessor"
 * 
 * Version / Date:
 *  2.3 / Januar 2018
 * 
 * Authors:
 *  Markus Hufschmid, Simon Gerber, Belinda Kneubuehler
 * 
 * File Name:
 *  userCode.c
 * 
 * Description:
 *  User Functions user_processData() and user_mainLoop()
*******************************************************************************/
#include <dsp.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "userConfig.h"
#include "globals.h"
#include "tlv320aic.h"
#include "IOconfig.h"
#include "userCode.h"
#include "controls.h"
#include "constants.h"
#include "globals.h"
#include "init.h"
#include "GUI.h"
#include "fft.h"
#include "DOGS.h"


/**************************************************************************
 * User Global Definitions
 ***************************************************************************/

volatile int8_t mode = 0;
FIRStruct FIRfilter;


/**************************************************************************
 * User Public Functions
 ***************************************************************************/

/**
 * This user function is called once in the begining. It can be used to
 * initialise user buffers ect.
 */
void user_init() {
    codec_setInput(LINE);
}

/**
 * This user function is called continuously in the main loop.
 */
void user_mainLoop() {
    if (controls.status) {
        processControls();
        update_GUI();
    }
    if (!fft_data_request && dispFFT) {
        computeFFT(fft_data);
        plotFFT();
        plotFilterBar(FFT_TOP_PAGE+1, 0);
        fft_data_request = 1;
    }
    set_xy_DOGS(0, 8*OVERFLOW_PAGE+7);
    if (overflowRightFlag || overflowLeftFlag) {
        puts_DOGS_inverted(" Overflow ");
    } else {
        puts_DOGS("          ");
    }
}

/**
 * This user function is called when new data is ready from the DMA.
 * @param sourceBuffer  is the buffer that contains the available input data
 * @param targetBuffer  is the buffer that the processed data has to be written into
 */
void user_processData(__eds__ int *sourceBuffer, __eds__ int *targetBuffer) {
    uint16_t i, index1, index2;
    int dummy;

    static fractional rxLeft[BUFFERLENGTH_DIV_2], rxRight[BUFFERLENGTH_DIV_2];
    static fractional txLeft[BUFFERLENGTH_DIV_2], txRight[BUFFERLENGTH_DIV_2];

    // copy sourceBuffer to leftSignalBuffer and rightSignalBuffer
    for (index1 = 0, index2 = 0; index1 < BUFFERLENGTH_DIV_2; index1++) {
        rxLeft[index1] = sourceBuffer[index2++];
        rxRight[index1] = sourceBuffer[index2++];
    }

    if (fft_data_request) {
        if (config.fft_channel == 0) {
            VectorCopy(BUFFERLENGTH_DIV_2, fft_data, rxLeft);
        } else {
            VectorCopy(BUFFERLENGTH_DIV_2, fft_data, rxRight);
        }
        fft_data_request = FALSE;
    }

    maxLeftValue = VectorMax(BUFFERLENGTH_DIV_2, rxLeft, &dummy);
    maxRightValue = VectorMax(BUFFERLENGTH_DIV_2, rxRight, &dummy);
    if (maxLeftValue > OVERFLOW_LIMIT) {
        overflowLeftFlag = TRUE;
        overflowTimer = 5000;
    }
    if (maxRightValue > OVERFLOW_LIMIT) {
        overflowRightFlag = TRUE;
        overflowTimer = 5000;
    }

    if (filterOn) {
        if (filter_type == BPFILTER) {
            FIR(BUFFERLENGTH_DIV_2, rxLeft, rxLeft, &leftFIRfilter);
            FIR(BUFFERLENGTH_DIV_2, rxRight, rxRight, &rightFIRfilter);
        }
        if (filter_type == LPHPFILTER) {
            FIR(BUFFERLENGTH_DIV_2, rxLeft, rxLeft, &leftFIRfilter);
            FIR(BUFFERLENGTH_DIV_2, rxRight, rxRight, &rightFIRfilter);
            FIR(BUFFERLENGTH_DIV_2, rxLeft, rxLeft, &leftFIRfilter2);
            FIR(BUFFERLENGTH_DIV_2, rxRight, rxRight, &rightFIRfilter2);
        }
    }
    if (autonotchOn) {
        for (i = 0; i < 64; i++) {
            rightAutonotchInput[i] = rightAutonotchDesired[i + 64];
        }
        for (i = 0; i < BUFFERLENGTH_DIV_2; i++) {
            rightAutonotchDesired[i] = rxRight[i];
        }

        FIRLMS(BUFFERLENGTH_DIV_2, rxRight, rightAutonotchInput, &rightAutonotchFilter, rightAutonotchDesired, 1638);
        //		FIRLMS(BUFFERLENGTH_DIV_2, rxRight, rightAutonotchInput, &rightAutonotchFilter, rightAutonotchDesired, 800);

        VectorSubtract(BUFFERLENGTH_DIV_2, rxRight, rxRight, rightAutonotchDesired);

        for (i = 0; i < 64; i++) {
            leftAutonotchInput[i] = leftAutonotchDesired[i + 64];
        }
        for (i = 0; i < BUFFERLENGTH_DIV_2; i++) {
            leftAutonotchDesired[i] = rxLeft[i];
        }

        FIRLMS(BUFFERLENGTH_DIV_2, rxLeft, leftAutonotchInput, &leftAutonotchFilter, leftAutonotchDesired, 1638);
        //		FIRLMS(BUFFERLENGTH_DIV_2, rxLeft, leftAutonotchInput, &leftAutonotchFilter, leftAutonotchDesired, 800);

        VectorSubtract(BUFFERLENGTH_DIV_2, rxLeft, rxLeft, leftAutonotchDesired);
    }


    VectorCopy(BUFFERLENGTH_DIV_2, txLeft, rxLeft);
    VectorCopy(BUFFERLENGTH_DIV_2, txRight, rxRight);

    //talk through: just copy input (rx) into output (tx) 
    for (i = 0; i < BUFFERLENGTH_DIV_2; i++) {
        txLeft[i] = rxLeft[i];
    }
    for (i = 0; i < BUFFERLENGTH_DIV_2; i++) {
        txRight[i] = rxRight[i];
    }

    //copy left and right txBuffer into targetBuffer
    for (index1 = 0, index2 = 0; index1 < BUFFERLENGTH_DIV_2; index1++) {
        targetBuffer[index2++] = txLeft[index1];
        targetBuffer[index2++] = txRight[index1]; 
    }
}
