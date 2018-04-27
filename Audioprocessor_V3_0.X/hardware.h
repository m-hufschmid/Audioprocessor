#ifndef _HARDWARE_H
#define	_HARDWARE_H

#include <xc.h>  

#define TOPBUTTON PORTEbits.RE6
#define MIDDLEBUTTON PORTEbits.RE5
#define BOTTOMBUTTON PORTEbits.RE4
#define LEFTENCODERBUTTON PORTEbits.RE2
#define RIGHTENCODERBUTTON PORTFbits.RF1

#define RIGHTENCODER_A PORTEbits.RE0
#define RIGHTENCODER_B PORTFbits.RF0
#define LEFTENCODER_A PORTEbits.RE3
#define LEFTENCODER_B PORTEbits.RE1

#endif	

