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
 *  userCode.h
 * 
 * Description:
 *  This file contains the user specific instructions that will be called at 
 *  the correct location in the main loop.
*******************************************************************************/

#ifndef _USERCODE_H
#define	_USERCODE_H

#ifdef	__cplusplus
extern "C"
{
#endif
#include <dsp.h>

void user_init();
void user_mainLoop();
void user_processData(__eds__ int *targetBuffer, __eds__ int *sourceBuffer);
// void testFunction(__eds__ int *);


//DEBUG for measurement
extern volatile int8_t mode;
extern uint16_t freq;

#ifdef	__cplusplus
}
#endif

#endif	/* _USERCODE_H */

