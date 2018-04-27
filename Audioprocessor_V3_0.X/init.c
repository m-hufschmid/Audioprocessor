/*******************************************************************************
 * Project:
 *  "Audioprocessor"
 * 
 * Version / Date:
 *  2.3 / Jan 2018
 *  
 * Authors:
 *  Markus Hufschmid, Simon Gerber, Belinda Kneubuehler
 * 
 * File Name:
 *  init.c
 * 
 * Description:
 *  This file initialises summons up the initialisations of the different 
 *  interfaces and contains the initialisation of the clock and a timer.
 *******************************************************************************/

#include "constants.h"
#include "xc.h"
#include <libpic30.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "init.h"
#include "IOconfig.h"
#include "i2c.h"
#include "tlv320aic.h"
#include "dma.h"
#include "dci.h"
#include "SPI.h"
#include "DOGS.h"


/**************************************************************************
 * Private Functions
 ***************************************************************************/

void clock_init(void) {
    int current_cpu_ipl;

    /* set fclk = 2*fcy = 2*69120000 =  M/(N1*N2)*fin with an 
     * input frequency of fin = 12288000
     */

    PLLFBD = 43; // M=45
    CLKDIVbits.PLLPOST = 0; // N2=2
    CLKDIVbits.PLLPRE = 0; // N1=2

    SET_AND_SAVE_CPU_IPL(current_cpu_ipl, 7); // disable interrupts

    // Clock switch to incorporate PLL
    __builtin_write_OSCCONH(0x03); // Initiate Clock Switch to
    // Primary Osc with PLL (NOSC=0b011)
    __builtin_write_OSCCONL(0x01); // Start clock switching

    RESTORE_CPU_IPL(current_cpu_ipl); // reenable interrupts 

    while (OSCCONbits.COSC != 0b011); // Wait for Clock switch to occur

    while (OSCCONbits.LOCK != 1) {
    }; // Wait for PLL to lock
}

void timer3_init(void) {
    // Configure Timer 3 to give an interrupt every 1 ms
    T3CONbits.TON = 0; // Disable Timer
    T3CONbits.TCKPS = 0b01; // Select 1:8 prescaler
    TMR3 = 0x0000; // Clear Timer Register
    PR3 = FCY / 80000 - 1; // Load Period Value ((1e-4)*FCY/8 - 1)
    IFS0bits.T3IF = 0; // Clear Interrupt Flag
    IEC0bits.T3IE = 1; // Enable Interrupt
    T3CONbits.TON = 1; // Start Timer
}

/**************************************************************************
 * Public Functions
 ***************************************************************************/

void init_all(void) {
    __delay_ms(5); /* make sure voltage is stable and 
                                         * codec is alive. */

    io_config();
    i2c_init(FCY_STARTUP);


    codec_init(); // init codec to forward external clock
    __delay_ms(5);

    clock_init(); // Switch to external clock
    i2c_init(FCY); // Init I2C for high system frequency

    dma_init();
    dci_init();
    timer3_init();
    
    init_SPI();
    init_DOGS();
}


