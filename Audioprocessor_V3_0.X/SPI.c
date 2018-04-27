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
 *  SPI.c
 * 
 * Description:
 *  functions for serial peripherial interface (SPI)
*******************************************************************************/
#include "xc.h"
#include "SPI.h"

void init_SPI(void){
	SPI1CON1 = 0;
	SPI1CON1bits.MSTEN = 1;		//Master Mode
	SPI1CON1bits.DISSCK = 0;	//SPI1 clock on SCK1 pin is enabled
	SPI1CON1bits.DISSDO = 0;	//SDO1 pin is controlled by the module
	SPI1CON1bits.MODE16 = 0;	//Communication is byte-wide (8 bits)
	SPI1CON1bits.SMP = 0;		//Input data is sampled at the middle of data output time
	SPI1CON1bits.CKE = 0;		//Serial output data changes on transition from Idle clock state to active clock state
	SPI1CON1bits.CKP = 1;		//Idle state for clock is a high level; active state is a low level
	SPI1CON1bits.PPRE = 2;		//Primary prescale 4:1
	SPI1CON1bits.SPRE = 7;		//Secondary prescale 1:1
	SPI1STATbits.SPIEN = 1;		//enable SPI module
}

void send_SPI(char data_out){
	while(SPI1STATbits.SPITBF);	//wait until SPIx Transmit Buffer Full Status bit (SPITBF)is cleared
	SPI1BUF = data_out;			//SPI1 Transmit Buffer Full Status bit (SPITBF) is set
}
