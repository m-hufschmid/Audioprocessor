#ifndef _SETUP_H
#define _SETUP_H

#include "globals.h"

void setupContrast(void);
void readConfigValuesFromEEPROM(config_struct *);
void storeConfigValuesToEEPROM(config_struct *);

#endif
