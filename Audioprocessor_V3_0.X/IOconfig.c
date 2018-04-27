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
 *  IOconfig.c
 * 
 * Description:
 *  This file maps the interfaces to the desired pins.
 *******************************************************************************/

#include "constants.h"
#include "xc.h"
#include <libpic30.h>

#include "IOconfig.h"

/**************************************************************************
 * Initiation
 ***************************************************************************/

void io_config(void) {
    // Unlock Registers
    __builtin_write_OSCCONL(OSCCON & ~(1 << 6));

    // Convert all I/O pins to digital.
    ANSELB = 0x0000;
    ANSELC = 0x0000;
    ANSELD = 0x0000;
    ANSELE = 0x0000;
    ANSELG = 0x0000;

    // GPIO (Outputs only)
    TRISEbits.TRISE7 = 0;   // RE7 is CS
    TRISGbits.TRISG7 = 0;   // RG7 is CD
    TRISGbits.TRISG9 = 0;   // RG9 is RST
        
    // SPI
    RPOR13bits.RP118R = 0b000101; // SPI.SDO1 tied to RP118 (Pin 4)
    RPOR14bits.RP120R = 0b000110; // SPI.SCK1 tied to RP120 (Pin 6)

    // DCI
    RPINR24bits.CSDIR = 46; // I2S.CSDI tied to RPI46 (Pin 29)
    RPINR25bits.COFSR = 47; // I2S.COFSIN tied to RPI47 (Pin30)
    RPOR9bits.RP100R = 0b001011; // I2S.CSDO tied to RP100 (Pin 31)
    RPINR24bits.CSCKR = 101; // I2S.CSCKIN tied to RP101 (Pin 32)
 
    // Lock Registers
    __builtin_write_OSCCONL(OSCCON | (1 << 6));
}
