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
 *  main.c
 * 
 * Description:
 *  main and interrupt functions
 *******************************************************************************/



#include "xc.h"
#include "constants.h"      // must be before #include <libpic30.h> because FCY is defined here
#include <libpic30.h>
#include <stdbool.h>
#include <stdint.h>
#include <p33EP512MC806.h>
#include <dsp.h>
#include <stdio.h>

#include "init.h"
#include "userCode.h"
#include "globals.h"
#include "GUI.h"
#include "controls.h"
#include "filterdesign.h"
#include "DEE Emulation 16-bit.h"
#include "setup.h"
#include "menu.h"
#include "DOGS.h"
#include "hardware.h"


/**************************************************************************
 * configuration bits
 ***************************************************************************/

/* Initial Oscillator Source = FRC
 * Two Speed Oscillator Startup = Start device with the user-selected
 * oscillator source*/
#pragma config FNOSC=FRC
#pragma config IESO=OFF

/* Clock Switching Mode = Clock switching is enabled; 
 * Fail-Safe Clock Monitor is disabled
 * OSC2 Pin Function bit = OSC2 is clock output
 * Primary Oscillator Mode Select bits = EC (External Clock) mode */
#pragma config FCKSM=CSECMD
#pragma config OSCIOFNC=OFF
#pragma config POSCMD=EC

/* Watchdog Timer Enable Mode = Watchdog Timer enabled/disabled 
 * by user software */
#pragma config FWDTEN=OFF

/* Power-on Reset Timer Value = 128ms & alternate I2C position off*/
#pragma config FPWRT=PWR128
#pragma config ALTI2C1=OFF

int main(void) {
    
    FIRStructInit(&leftFIRfilter, NTAPS, coefficients, COEFFS_IN_DATA, leftDelayBuffer);
    FIRStructInit(&rightFIRfilter, NTAPS, coefficients, COEFFS_IN_DATA, rightDelayBuffer);
    FIRStructInit(&leftFIRfilter2, NTAPS, coefficients2, COEFFS_IN_DATA, leftDelayBuffer2);
    FIRStructInit(&rightFIRfilter2, NTAPS, coefficients2, COEFFS_IN_DATA, rightDelayBuffer2);

    FIRStructInit(&rightAutonotchFilter, NTAPS_NOTCH, coefficients3, COEFFS_IN_DATA, rightDelayBuffer3);
    FIRStructInit(&leftAutonotchFilter, NTAPS_NOTCH, coefficients3, COEFFS_IN_DATA, leftDelayBuffer3);

    FIRDelayInit(&leftFIRfilter);
    FIRDelayInit(&rightFIRfilter);
    FIRDelayInit(&leftFIRfilter2);
    FIRDelayInit(&rightFIRfilter2);
    FIRDelayInit(&leftAutonotchFilter);
    FIRDelayInit(&rightAutonotchFilter);

    init_all();

    user_init();
    __delay_ms(100); // make sure voltage is stable and codec is alive

    DataEEInit();
    if (DataEERead(DATA_EE_SIZE - 1) == 0xFFFF) { //no config data has been stored in EEPROM yet
        storeConfigValuesToEEPROM(&config);
        DataEEWrite(0x0000, DATA_EE_SIZE - 1);
    }
    readConfigValuesFromEEPROM(&config);
    setInputGain(config.gain_input);
    setHeadphoneGain(config.gain_headphone);
    send_cmd_DOGS(0x81);
    send_cmd_DOGS(config.contrast_value);

    // Check whether user whishes to set up the contrast at startup
	if (LEFTENCODERBUTTON == 0){
		setupContrast();
	}
    
    iLPFilter = N_LP_FILTERS - 1;
    iHPFilter = 0;
    build_filter(HIGHPASS);
    build_filter(LOWPASS);

    iBPFilter = N_BP_FILTERS - 1;
    fc = 700;
    build_filter(BANDPASS);

    build_menu();
    build_GUI();
    update_GUI();

    while (1) {
        user_mainLoop();
    }
    return 0;
}

/**************************************************************************
 * Interrupt Routines
 ***************************************************************************/
void __attribute__((__interrupt__, no_auto_psv)) _DMA2Interrupt(void) {
//DMA 2 interrupt function. Will be called once the DMA buffers are filled
    
    rxBufferIndicator = !rxBufferIndicator; // Toggle the indicator
    if (rxBufferIndicator) {
        user_processData(rxBufferA, txBufferA);
    } else {
        user_processData(rxBufferB, txBufferB);
    }

    IFS1bits.DMA2IF = 0; // Clear DMA channel 2 interrupt status flag
}

void __attribute__((__interrupt__, __shadow__, no_auto_psv)) _T3Interrupt(void) {
// Timer 3 interrupt function. Will be called every 0.1 ms
    
    controls = updateControls();

    //decrement button timers for debouncing or key repetition
    if (topButton.timer != 0) {
        topButton.timer--;
    }
    if (middleButton.timer != 0) {
        middleButton.timer--;
    }
    if (bottomButton.timer != 0) {
        bottomButton.timer--;
    }
    if (leftEncoderButton.timer != 0) {
        leftEncoderButton.timer--;
    }
    if (rightEncoderButton.timer != 0) {
        rightEncoderButton.timer--;
    }
    if (REncTimer != 0) {
        REncTimer -= 1;
    }
    if (LEncTimer != 0) {
        LEncTimer -= 1;
    }
    if (overflowTimer != 0) {
        overflowTimer -= 1;
    } else {
        overflowRightFlag = false;
        overflowLeftFlag = false;
    }
    IFS0bits.T3IF = 0; //Clear Timer3 interrupt flag
    return;

}

