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
 *  menu.c
 * 
 * Description:
 *  This file implements the functions of the configuration menu
 *******************************************************************************/

#include "DOGS.h"
#include "GenericTypeDefs.h"
#include "menu.h"
#include "globals.h"
#include "tlv320aic.h"
#include <stdio.h>
#include "setup.h"

#define MAX_MENU_TITLE	3

enum menuTitles {MENU_FFT, MENU_GAIN, MENU_CONTRAST, MENU_EXIT};

#define MAX_FFT_CHANNEL 1
#define MAX_FFT_WINDOW	2
#define MAX_FFT_DETECT	1
#define MAX_GAIN_INPUT	31
#define MAX_GAIN_HEADPHONE	127
#define MIN_GAIN_HEADPHONE	48

enum exitStates {EXIT_NONE=0, EXIT_CANCEL, EXIT_SAVE};

char *fft_channels[] = {"LEFT  ","RIGHT "};
char *fft_windows[] = {"RECT  ","KAISER","CHEBY "};
char *fft_detects[] = {"PEAK  ","SMOOTH"};


const char MENU_PIC_01[8][9] = 
	{{192,	192,	192,	64,	192,	192,	192,	192,	192},
	 {127,	127,	127,	64,	96,	113,	123,	127,	127},
	 {0,	0,	0,	0,	0,	0,	0,	0,	0},
	 {240,	240,	240,	112,	48,	16,	240,	240,	240},
	 {31,	31,	30,	28,	24,	16,	31,	31,	31},
	 {0,	0,	0,	0,	0,	0,	0,	0,	0},
	 {254,	238,	206,	142,	14,	142,	206,	238,	254},
	 {3,	3,	3,	3,	3,	3,	3,	3,	3}};

const char MENU_PIC_02[8][9] = 
	{{192,	192,	192,	192,	64,	192,	192,	192,	192},
	 {127,	123,	123,	123,	64,	123,	123,	123,	127},
	 {0,	0,	0,	0,	0,	0,	0,	0,	0},
	 {240,	240,	240,	240,	240,	240,	240,	240,	240},
	 {31,	30,	30,	30,	30,	30,	30,	30,	31},
	 {0,	0,	0,	0,	0,	0,	0,	0,	0},
	 {254,	238,	206,	142,	14,	142,	206,	238,	254},
	 {3,	3,	3,	3,	3,	3,	3,	3,	3}};

const char MENU_PIC_03[8][9] = 
	{{192, 	192, 	192, 	192, 	192, 	64, 	192, 	192, 	192},
	 {127, 	103, 	91, 	103, 	127, 	64, 	103, 	91, 	127},
	 {0,	0,	0,	0,	0,	0,	0,	0,	0},
	 {0,	0,	0,	0,	0,	0,	0,	0,	0},
	 {0,	0,	0,	0,	0,	0,	0,	0,	0},
	 {0,	0,	0,	0,	0,	0,	0,	0,	0},
	 {254, 	238, 	206, 	142, 	14, 	142, 	206, 	238, 	254},
	 {3,	3,	3,	3,	3,	3,	3,	3,	3}};


void menu(void){
	int menuTitle = MENU_FFT;
	int exitStatus = 0;

	buildMenu(menuTitle);
	while (exitStatus == EXIT_NONE){
		if (topButton.counter){
			topButton.counter = 0;
			menuTitle++;
			if (menuTitle > MAX_MENU_TITLE){
				menuTitle = 0;
			}
			buildMenu(menuTitle);
		}
		if (middleButton.counter){
			middleButton.counter = 0;
			menuTitle--;
			if (menuTitle < 0){
				menuTitle = MAX_MENU_TITLE;
			}
			buildMenu(menuTitle);
		}
		if (bottomButton.counter){
			bottomButton.counter = 0;
			exitStatus = updateMenu(menuTitle);
		}
		if (menuTitle == MENU_GAIN){
			plotGainBar(2,6);
		}
	}
	if (exitStatus == EXIT_SAVE){
		storeConfigValuesToEEPROM(&config);
	}
 	readConfigValuesFromEEPROM(&config);
}

void buildMenu(int menuTitle){
	BYTE page, column;
	char s[6];

	clear_DOGS();
	for (page=0; page<8; page++){
		set_page_DOGS(page);
		set_column_DOGS(93);
		for (column=93; column < 102; column++){
			send_data_DOGS(MENU_PIC_01[page][column-93]);
		}
	}
	switch (menuTitle){
		case MENU_FFT:
			set_xy_DOGS(0,7);
			puts_DOGS_inverted("      FFT      ");
			set_xy_DOGS(0,23);
			puts_DOGS("Channel");
			set_xy_DOGS(48,23);
			puts_DOGS(fft_channels[config.fft_channel]);
			set_xy_DOGS(0,39);
			puts_DOGS("Window");
			set_xy_DOGS(48,39);
			puts_DOGS(fft_windows[config.fft_window]);
			set_xy_DOGS(0,55);
			puts_DOGS("Detect");
			set_xy_DOGS(48,55);
			puts_DOGS(fft_detects[config.fft_detect]);
			break;
		case MENU_GAIN:
			set_xy_DOGS(0,7);
			puts_DOGS_inverted("     GAIN      ");
			set_xy_DOGS(0,23);
			puts_DOGS("L");
			set_xy_DOGS(0,31);
			puts_DOGS("R");
			set_xy_DOGS(0,47);
			puts_DOGS("Input        dB");
			gain_input_to_string(s);
			set_xy_DOGS(48,47);
			puts_DOGS(s);
			set_xy_DOGS(0,63);
			puts_DOGS("Headph.      dB");
			gain_headphone_to_string(s);
			set_xy_DOGS(48,63);
			puts_DOGS(s);
			break;
		case MENU_EXIT:
			set_xy_DOGS(0,7);
			puts_DOGS_inverted("     EXIT      ");
			set_xy_DOGS(0,23);
			puts_DOGS("Cancel");
			set_xy_DOGS(0,39);
			puts_DOGS("Save");
			break;
		case MENU_CONTRAST:
			set_xy_DOGS(0,7);
			puts_DOGS_inverted("   CONTRAST    ");
			set_xy_DOGS(0,23);
			puts_DOGS("Use right");
			set_xy_DOGS(0,31);
			puts_DOGS("encoder to");
			set_xy_DOGS(0,39);
			puts_DOGS("adjust contrast");
			while (topButton.counter == 0 && middleButton.counter == 0){
				if (controls.flags.REnc){
					controls.flags.REnc = 0;
					config.contrast_value += REncValue;
					REncValue = 0;
					if (config.contrast_value < 0){
						config.contrast_value = 0;
					}
					if (config.contrast_value > 63){
						config.contrast_value = 63;
					}
					send_cmd_DOGS(0x81);
					send_cmd_DOGS(config.contrast_value);
				}
			}
			bottomButton.counter = 0;
			break;
	}
}

int updateMenu(int menuTitle){
	BYTE page, column; 
	int exitStatus = 0;
	char s[6];
	

	for (page=0; page<8; page++){
		set_page_DOGS(page);
		set_column_DOGS(93);
		for (column=93; column < 102; column++){
			if (menuTitle == MENU_EXIT){
				send_data_DOGS(MENU_PIC_03[page][column-93]);
			} else  {
				send_data_DOGS(MENU_PIC_02[page][column-93]);
			} 		
		}
	}
	
	switch (menuTitle){
		case MENU_FFT:
			set_xy_DOGS(48,23);
			puts_DOGS_inverted(fft_channels[config.fft_channel]);
			while (bottomButton.counter == 0){
				if (topButton.counter){
					topButton.counter = 0;
					config.fft_channel++;
					if (config.fft_channel > MAX_FFT_CHANNEL){
						config.fft_channel = 0;
					}
					set_xy_DOGS(48,23);
					puts_DOGS_inverted(fft_channels[config.fft_channel]);
				}
				if (middleButton.counter){
					middleButton.counter = 0;
					config.fft_channel--;
					if (config.fft_channel < 0){
						config.fft_channel =  MAX_FFT_CHANNEL;
					}
					set_xy_DOGS(48,23);
					puts_DOGS_inverted(fft_channels[config.fft_channel]);
				}
			}
			bottomButton.counter = 0;
			set_xy_DOGS(48,23);
			puts_DOGS(fft_channels[config.fft_channel]);

			set_xy_DOGS(48,39);
			puts_DOGS_inverted(fft_windows[config.fft_window]);
			while (bottomButton.counter == 0){
				if (topButton.counter){
					topButton.counter = 0;
					config.fft_window++;
					if (config.fft_window > MAX_FFT_WINDOW){
						config.fft_window = 0;
					}
					set_xy_DOGS(48,39);
					puts_DOGS_inverted(fft_windows[config.fft_window]);
				}
				if (middleButton.counter){
					middleButton.counter = 0;
					config.fft_window--;
					if (config.fft_window < 0){
						config.fft_window =  MAX_FFT_WINDOW;
					}
					set_xy_DOGS(48,39);
					puts_DOGS_inverted(fft_windows[config.fft_window]);
				}
			}
			bottomButton.counter = 0;
			set_xy_DOGS(48,39);
			puts_DOGS(fft_windows[config.fft_window]);
			
			set_xy_DOGS(48,55);
			puts_DOGS_inverted(fft_detects[config.fft_detect]);
			while (bottomButton.counter == 0){
				if (topButton.counter){
					topButton.counter = 0;
					config.fft_detect++;
					if (config.fft_detect > MAX_FFT_DETECT){
						config.fft_detect = 0;
					}
					set_xy_DOGS(48,55);
					puts_DOGS_inverted(fft_detects[config.fft_detect]);
				}
				if (middleButton.counter){
					middleButton.counter = 0;
					config.fft_detect--;
					if (config.fft_detect < 0){
						config.fft_detect =  MAX_FFT_DETECT;
					}
					set_xy_DOGS(48,55);
					puts_DOGS_inverted(fft_detects[config.fft_detect]);
				}
			}
			bottomButton.counter = 0;
			set_xy_DOGS(48,55);
			puts_DOGS(fft_detects[config.fft_detect]);
			break;
		case MENU_GAIN:
			gain_input_to_string(s);
			set_xy_DOGS(48,47);
			puts_DOGS_inverted(s);			
			while (bottomButton.counter == 0){
				if (topButton.counter){
					topButton.counter = 0;
					config.gain_input++;
					if (config.gain_input > MAX_GAIN_INPUT){
						config.gain_input = MAX_GAIN_INPUT;
					}
					gain_input_to_string(s);
					set_xy_DOGS(48,47);
					puts_DOGS_inverted(s);
					setInputGain(config.gain_input);
				}
				if (middleButton.counter){
					middleButton.counter = 0;
					config.gain_input--;
					if (config.gain_input < 0){
						config.gain_input =  0;
					}
					gain_input_to_string(s);
					set_xy_DOGS(48,47);
					puts_DOGS_inverted(s);
					setInputGain(config.gain_input);
				}
				plotGainBar(2,6);
			}
			bottomButton.counter = 0;
			gain_input_to_string(s);
			set_xy_DOGS(48,47);
			puts_DOGS(s);

			gain_headphone_to_string(s);
			set_xy_DOGS(48,63);
			puts_DOGS_inverted(s);
			while (bottomButton.counter == 0){
				if (topButton.counter){
					config.gain_headphone += topButton.counter;
					topButton.counter = 0;
					if (config.gain_headphone > MAX_GAIN_HEADPHONE){
						config.gain_headphone = MAX_GAIN_HEADPHONE;
					}
					gain_headphone_to_string(s);
					set_xy_DOGS(48,63);
					puts_DOGS_inverted(s);
					setHeadphoneGain(config.gain_headphone);
				}
				if (middleButton.counter){
					config.gain_headphone -= middleButton.counter;
					middleButton.counter = 0;
					if (config.gain_headphone < MIN_GAIN_HEADPHONE){
						config.gain_headphone =  MIN_GAIN_HEADPHONE;
					}
					gain_headphone_to_string(s);
					set_xy_DOGS(48,63);
					puts_DOGS_inverted(s);
					setHeadphoneGain(config.gain_headphone);
				}
				plotGainBar(2,6);
			}
			bottomButton.counter = 0;
			gain_headphone_to_string(s);
			set_xy_DOGS(48,63);
			puts_DOGS(s);
			break;
		case MENU_EXIT:
			set_xy_DOGS(0,23);
			puts_DOGS_inverted("Cancel");
			while (bottomButton.counter == 0 && exitStatus == 0){
				if (topButton.counter){
					topButton.counter = 0;
					exitStatus = EXIT_CANCEL;
				}
			}
			bottomButton.counter = 0;
			set_xy_DOGS(0,23);
			puts_DOGS("Cancel");
			set_xy_DOGS(0,39);
			puts_DOGS_inverted("Save");
			while (bottomButton.counter == 0 && exitStatus == 0){
				if (topButton.counter){
					topButton.counter = 0;
					exitStatus = EXIT_SAVE;
				}
			}
			bottomButton.counter = 0;
			set_xy_DOGS(0,39);
			puts_DOGS("Save");
			break;
		case MENU_CONTRAST:
			break;
	}
	for (page=0; page<8; page++){
		set_page_DOGS(page);
		set_column_DOGS(93);
		for (column=93; column < 102; column++){
			send_data_DOGS(MENU_PIC_01[page][column-93]);
		}
	}
	return exitStatus;
}


void gain_input_to_string(char *s){
	int value, n;

	value = 3*(config.gain_input - 23);
	if (value >= 0){
		n = sprintf(s, "%+3d",value>>1);
	} else {
		n = sprintf(s, "%+3d",(value+1)>>1);
	}
	s[n] = '.';
	if (value & 0x01){
		s[n+1] = '5';
	} else {
		s[n+1] = '0';
	}
	s[n+2] = 0;
}	

void gain_headphone_to_string(char *s){
	int value, n;

	value = config.gain_headphone - 121;
	n = sprintf(s, "%+3d",value);
	s[n] = '.';
	s[n+1] = '0';
	s[n+2] = 0;
}	

void setInputGain(int value){
    codec_setLineInVol(value);
}

void setHeadphoneGain(int value){
    codec_setHeadphoneVol(value);
}

void plotGainBar(int page, int column){
	int indexLeftHigh, indexRightHigh, i;

	indexLeftHigh = maxLeftValue>>9;
	if (indexLeftHigh > 63){
		indexLeftHigh =63;
	}
	indexRightHigh = maxRightValue>>9;
	if (indexRightHigh > 63){
		indexRightHigh =63;
	}
	set_page_DOGS(page);
	set_column_DOGS(column);
	send_data_DOGS(0xFF);
	send_data_DOGS(0x81);
    for (i = 0; i < 64; i++){
        if (i < indexLeftHigh){
            send_data_DOGS(0xBD);
        } else {
            send_data_DOGS(0x81);
        }
    }
	send_data_DOGS(0xFF);
	set_xy_DOGS(78,23);
	if (overflowLeftFlag){
		puts_DOGS_inverted("OF");
	} else {
		puts_DOGS("  ");
	}
	set_page_DOGS(page+1);
	set_column_DOGS(column);
	send_data_DOGS(0xFF);
	send_data_DOGS(0x81);
    for (i = 0; i < 64; i++){
        if (i < indexRightHigh){
            send_data_DOGS(0xBD);
        } else {
            send_data_DOGS(0x81);
        }
    }
	send_data_DOGS(0xFF);
	set_xy_DOGS(78,31);
	if (overflowRightFlag){
		puts_DOGS_inverted("OF");
	} else {
		puts_DOGS("  ");
	}
}
