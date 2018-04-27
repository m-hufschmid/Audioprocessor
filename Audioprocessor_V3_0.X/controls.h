#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include "globals.h"

CONTROLSTATUS updateControls(void);
void processControls(void);
void buttonStateMachine(buttonStruct*, int);


#endif

