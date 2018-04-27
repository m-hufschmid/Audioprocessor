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
 *  i2c.c
 * 
 * Description:
 *  This file initialises the I2C interface and provides the functions to read
 *  and write commands over the interface.
*******************************************************************************/

#include "xc.h"
#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"
#include "constants.h"

/**************************************************************************
 * Initiation
 ***************************************************************************/

void i2c_init(uint32_t fsys)
{
    I2C1CONbits.A10M = 0;       // 7-bit address
   
    I2C1BRG = (fsys/I2C_CLK_SPEED - 0.120*(fsys/1000000))- 2; 

    I2C1CONbits.I2CEN = 1;      // Enable I2C
}

/**************************************************************************
 * Private Functions
 ***************************************************************************/

void StartI2C1(void)
{
    I2C1CONbits.SEN = 1;        // initiate Start on SDA and SCL pins 
}

void WaitForIdleI2C1(void)
{
                                // Wait until I2C Bus is Inactive 
    while (I2C1CONbits.SEN || I2C1CONbits.PEN || I2C1CONbits.RCEN ||
            I2C1CONbits.ACKEN || I2C1STATbits.TRSTAT);
}


void RepStartI2C1()
{
    WaitForIdleI2C1();
    I2C1CONbits.RSEN=1;
}

char MasterWriteI2C1(uint8_t data_out)
{
    I2C1TRN = data_out;

    if (I2C1STATbits.IWCOL)     // If write collision occurs,return -1
        return -1;
    else
    {
        return 0;
    }
}

char MasterReadI2C1(bool ack)
{
    
 char i2cReadData;

 I2C1CONbits.RCEN=1;
 WaitForIdleI2C1();
 i2cReadData = I2C1RCV;
 
 if ( ack )
  {
  I2C1CONbits.ACKDT=0;
  }
 else
  {
  I2C1CONbits.ACKDT=1;
  }
 
  I2C1CONbits.ACKEN=1;               // send acknowledge sequence
  WaitForIdleI2C1();      // Wait to complete
  
 return( i2cReadData );
}

void StopI2C1(void)
{
    I2C1CONbits.PEN = 1; /* initiate Stop on SDA and SCL pins */
}

/**************************************************************************
 * Public Functions
 ***************************************************************************/

int i2c_write(uint8_t *i2cData, uint16_t DataSz)
{
    int result = 0, Index = 0;


    StartI2C1();            // Send the Start Bit
    WaitForIdleI2C1();      // Wait to complete
    while (DataSz)
    {
        MasterWriteI2C1(i2cData[Index++]);
        WaitForIdleI2C1();  // Wait to complete
        DataSz--;
        /* ACKSTAT is 0 when slave acknowledge. 
        * if 1 then slave has not acknowledge the data.*/
        if (I2C1STATbits.ACKSTAT)
        {
            result = -1;
            break;
        }
    }
    StopI2C1();             // Send the Stop condition
    WaitForIdleI2C1();      // Wait to complete
    return result;
}

void i2c_read(uint8_t address, uint16_t registerAdress, uint8_t *buffer, uint8_t size)
{
    StartI2C1();
    WaitForIdleI2C1();
    MasterWriteI2C1((address<<1) | 0x01);
    WaitForIdleI2C1();
    MasterWriteI2C1(registerAdress>>8);
    WaitForIdleI2C1();
    MasterWriteI2C1(registerAdress&0xFF);
    WaitForIdleI2C1();      // Wait to complete
    while(size--)
    {
        *(buffer++) = MasterReadI2C1(size>0);  //size>0 -> no ack after last byte
    }
    StopI2C1();
    WaitForIdleI2C1();      // Wait to complete
}