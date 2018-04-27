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
 *  setup.c
 * 
 * Description:
 *  Different setup functions
 *******************************************************************************/

#include <xc.h>
#include "DOGS.h"
#include "GenericTypeDefs.h"
#include "controls.h"
#include "globals.h"
#include "DEE Emulation 16-bit.h"
#include "setup.h"

const BYTE CONTRASTMENU[8][25] = 
	{{192, 192, 64, 64, 64, 64, 192, 192, 192, 64, 192, 192, 192, 64, 192, 192, 192, 64, 192, 64, 64, 64, 64, 64, 192},
	 {127, 92, 91, 91, 91, 103, 127, 65, 122, 123, 122, 65, 127, 112, 111, 95, 111, 112, 127, 64, 91, 91, 95, 95, 127},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {254, 6, 250, 250, 250, 118, 254, 2, 238, 222, 190, 2, 254, 6, 250, 250, 250, 118, 254, 2, 254, 254, 254, 254, 254},
	 {3, 3, 2, 2, 2, 3, 3, 2, 3, 3, 3, 2, 3, 3, 2, 2, 2, 3,	3,	2,	2,	2,	2,	2,	3}};

void setupContrast(void){
	BYTE page,column;
	int contrast;
	
	for (page=0; page<8; page++){
		set_page_DOGS(page);
		set_column_DOGS(0);
		for (column=0; column<72; column++){
			if (((column>>3)+page) & 0x01){
				send_data_DOGS(0xFF);
			} else {
				send_data_DOGS(0x00);
			}
		}
		for (column=72; column<77; column++){
			send_data_DOGS(0x00);
		}
		for (column=77; column<102; column++){
			send_data_DOGS(CONTRASTMENU[page][column-77]);
		}
	}
	contrast = config.contrast_value;
	while (!topButton.counter && !bottomButton.counter){
		if (controls.flags.REnc){
			controls.flags.REnc = 0;
			contrast += REncValue;
			REncValue = 0;
			if (contrast < 0){
				contrast = 0;
			}
			if (contrast > 63){
				contrast = 63;
			}
			send_cmd_DOGS(0x81);
			send_cmd_DOGS(contrast);
		}
	}
    if (topButton.counter){
        readConfigValuesFromEEPROM(&config);
        config.contrast_value = contrast;
        storeConfigValuesToEEPROM(&config);
        topButton.counter = 0;
    }
    bottomButton.counter = 0;
    leftEncoderButton.counter = 0;
}	 

void readConfigValuesFromEEPROM(config_struct *config){
	int i;
	int *intPtr;

	intPtr = (int*)config;
	for (i=0; i < sizeof(config_struct)/2; i++){
		intPtr[i] = DataEERead(i);
	}		
}

void storeConfigValuesToEEPROM(config_struct *config){
	int i;
	int *intPtr;

	intPtr = (int*)config;
	for (i=0; i < sizeof(config_struct)/2; i++){
		DataEEWrite(*intPtr++, i);
	}		
}

