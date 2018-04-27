/*******************************************************************************
 * Project:
 *  "Audioprocessor"
 * 
 * Version / Date:
 *  2.3 / Jan 2018
 * 
 * Authors:
 *  Markus Hufschid, Simon Gerber, Belinda Kneubuehler
 * 
 * File Name:
 *  dma.c
 * 
 * Description:
 *  This file configures the DMA to work with the I2S interface.
*******************************************************************************/

#include "xc.h"
#include "dma.h"
#include "init.h"


/**************************************************************************
 * Initiation
 ***************************************************************************/

void dma_init(void)
{
    // DMA 0 - RAM to DCI  
    DMA0CONbits.SIZE = 0;       // Data Transfer size: Word  
    DMA0CONbits.DIR = 1;        // Transfer Direction: From RAM to DCI  
    DMA0CONbits.AMODE = 0;      /* DMA Channel Adressing Mode: 
                                 * Register Indirect with Post-Increment mode*/ 
    DMA0CONbits.MODE = 2;       /* Channel Operating Mode: Continuous, 
                                 * Ping-Pong modes enabled  */
    DMA0CONbits.HALF = 0;       /* Block Transfer Interrupt: Initiate interrupt 
                                 * when all of the data has been moved */
    DMA0CONbits.NULLW = 0;      /* Null Data Peripheral Write Mode:
                                 * Normal Operation */
    DMA0REQbits.FORCE = 0;      /* Automatic DMA transfer initiation
                                 * by DMA request */
    DMA0REQbits.IRQSEL = 0x3C;  // DMA Peripherial IRQ Number: DCI 
    DMA0STAH = 0x00;            // __builtin_dmaoffset(txBufferA) >> 8;
    DMA0STAL = __builtin_dmaoffset(txBufferA);
    DMA0STBH = 0x00;            //__builtin_dmaoffset(txBufferB) >> 8;
    DMA0STBL = __builtin_dmaoffset(txBufferB);
    DMA0PAD = (int) &TXBUF0;    // DMA Channel 0 Peripherial Address  
    DMA0CNT = BUFFERLENGTH - 1; /* DMA Channel 0 Transfer Count: 
                                 * Transfer FRAMELENGTH elements */ 

    // DMA 2 - DCI to RAM  
    DMA2CONbits.SIZE = 0;       // Data Transfer size: Word  
    DMA2CONbits.DIR = 0;        // Transfer Direction: From DCI to RAM  
    DMA2CONbits.HALF = 0;       /* DMA Channel Adressing Mode: Register 
                                 * Indirect with Post-Increment mode*/  
    DMA2CONbits.NULLW = 0;      /* Null Data Peripheral Write Mode: 
                                 * Normal Operation */
    DMA2CONbits.AMODE = 0;      /* DMA Channel Adressing Mode: Register 
                                 * Indirect with Post-Increment mode */ 
    DMA2CONbits.MODE = 2;       /* Channel Operating Mode: Continuous, 
                                 * Ping-Pong modes enabled */
    DMA2REQbits.FORCE = 0;      /* Automatic DMA transfer initiation 
                                 * by DMA request */
    DMA2REQbits.IRQSEL = 0x3C;  // DMA Peripherial IRQ Number: DCI 
    DMA2STAH = 0x00;            //__builtin_dmaoffset(rxBufferA) >> 8;
    DMA2STAL = __builtin_dmaoffset(rxBufferA);
    DMA2STBH = 0x00;            //__builtin_dmaoffset(rxBufferB) >> 8;
    DMA2STBL = __builtin_dmaoffset(rxBufferB);
    DMA2PAD = (int) &RXBUF0;    // DMA Channel 2 Peripherial Address  
    DMA2CNT = BUFFERLENGTH - 1; /* DMA Channel 2 Transfer Count: 
                                 * Transfer FRAMELENGTH elements */
    IFS1bits.DMA2IF = 0;        // Clear DMA Channel 2 Interrupt Flag  
    IEC1bits.DMA2IE = 1;        // Enable DMA Channel 2 Interrupt  

    DMA0CONbits.CHEN = 1;       // Enable DMA channel 0  
    DMA2CONbits.CHEN = 1;       // Enable DMA channel 2  
}

