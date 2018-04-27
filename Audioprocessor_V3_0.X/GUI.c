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
 *  This file implements the graphical user interface
 *******************************************************************************/

#include <stdbool.h>
#include "GUI.h"
#include "DOGS.h"
#include "GenericTypeDefs.h"
#include "globals.h"

const unsigned char OFF_PIC[16] = {127,99,93,93,99,127,65,117,117,125,127,65,117,117,125,127};
const unsigned char ON_PIC[16] = {127,127,127,99,93,93,99,127,127,65,115,103,65,127,127,127};
const unsigned char LH_PIC[16] = {127,65,95,95,95,127,119,119,119,119,127,65,119,119,65,127};
const unsigned char BP_PIC[16] = {127,127,127,65,85,85,107,127,127,65,117,117,123,127,127,127};
const unsigned char MENU_BAR_PIC[8][25] = {
{192, 	64, 	64, 	64, 	64, 	64, 	192, 	64, 	192, 	192, 	192, 	192, 	192, 	64, 	64, 	64, 	64, 	64, 	192, 	64, 	64, 	64, 	64, 	192, 	192},
{127, 	64, 	123, 	123, 	123, 	127, 	127, 	64, 	95, 	95, 	95, 	95, 	127, 	127, 	127, 	64, 	127, 	127, 	127, 	64, 	123, 	115, 	107, 	92, 	127},
{0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0},
{240, 	16, 	112, 	240, 	240, 	16, 	240, 	208, 	208, 	16, 	208, 	208, 	240, 	48, 	208, 	208, 	208, 	176, 	240, 	16, 	240, 	240, 	240, 	16, 	240},
{31, 	16, 	31, 	30, 	29, 	16, 	31, 	31, 	31, 	16, 	31, 	31, 	31, 	24, 	23, 	23, 	23, 	27, 	31, 	16, 	30, 	30, 	30, 	16, 	31},
{0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0},
{254, 	2, 	250, 	250, 	250, 	6, 	254, 	254, 	250, 	2, 	250, 	254, 	254, 	230, 	218, 	218, 	218, 	58, 	254, 	2, 	218, 	218, 	218, 	230, 	254},
{3, 	2, 	2, 	2, 	2, 	3, 	3, 	3, 	2, 	2, 	2, 	3, 	3, 	2, 	2, 	2, 	2, 	3, 	3, 	2, 	3, 	3, 	3, 	3, 	3}
};

void build_menu(){
	BYTE page, column;
	 
	clear_DOGS();

	clear_DOGS();
	for (page=0; page<8; page++){
		set_page_DOGS(page);
		set_column_DOGS(77);
		for (column=77; column < 102; column++){
			send_data_DOGS(MENU_BAR_PIC[page][column-77]);
		}
	}
}

void build_GUI(){
//	set_xy_DOGS(0, 7);
//	puts_DOGS("Filter_____");
}

void update_GUI(){
	int i;

  	set_xy_DOGS(0, 8*FILTER_PAGE+7);
    if (filter_type == BPFILTER){
        puts_DOGS("Bandpass");
    } else {
        puts_DOGS("Low-High");
    }
	set_page_DOGS(FILTER_PAGE);
	set_column_DOGS(50);
	for (i=0; i<16;i++){
		if (filterOn){
			send_data_DOGS(ON_PIC[i]);
		} else {
			send_data_DOGS(OFF_PIC[i]);
		}
	}
	set_page_DOGS(NOTCH_PAGE);
	set_column_DOGS(50);
	for (i=0; i<16;i++){
		if (autonotchOn){
			send_data_DOGS(ON_PIC[i]);
		} else {
			send_data_DOGS(OFF_PIC[i]);
		}
	}
	if (!dispFFT){
		switch (filter_type){
		case BPFILTER: 
			set_xy_DOGS(0, 8*DISPLAY_PARAMETERS_PAGE+7);
			puts_DOGS("}Type Bandp");
			set_xy_DOGS(0, 8*(DISPLAY_PARAMETERS_PAGE+1)+7);
			puts_DOGS("}fc");
			set_xy_DOGS(0,8*(DISPLAY_PARAMETERS_PAGE+2)+7);
			puts_DOGS("}BW");
	
			set_xy_DOGS(18,8*(DISPLAY_PARAMETERS_PAGE+1)+7);
			write_int_DOGS(fc);
			set_xy_DOGS(54,8*(DISPLAY_PARAMETERS_PAGE+1)+7);
			puts_DOGS("Hz");
			set_xy_DOGS(18,8*(DISPLAY_PARAMETERS_PAGE+2)+7);
			write_int_DOGS(bandwidth);
			set_xy_DOGS(54,8*(DISPLAY_PARAMETERS_PAGE+2)+7);
			puts_DOGS("Hz");
			break;
		case LPHPFILTER:
			set_xy_DOGS(0, 8*DISPLAY_PARAMETERS_PAGE+7);
			puts_DOGS("}Type LP-HP");
			set_xy_DOGS(0, 8*(DISPLAY_PARAMETERS_PAGE+1)+7);
			puts_DOGS("}fL");
			set_xy_DOGS(0,8*(DISPLAY_PARAMETERS_PAGE+2)+7);
			puts_DOGS("}fH");
	
			set_xy_DOGS(18,8*(DISPLAY_PARAMETERS_PAGE+1)+7);
			write_int_DOGS(fLowpass);
			set_xy_DOGS(54,8*(DISPLAY_PARAMETERS_PAGE+1)+7);
			puts_DOGS("Hz");
			set_xy_DOGS(18,8*(DISPLAY_PARAMETERS_PAGE+2)+7);
			write_int_DOGS(fHighpass);
			set_xy_DOGS(54,8*(DISPLAY_PARAMETERS_PAGE+2)+7);
			puts_DOGS("Hz");
			break;
		}
	}
	set_xy_DOGS(0,8*NOTCH_PAGE+7);
	puts_DOGS("A-Notch");
		

}

void plotFilterBar(int page, int column){
	int indexLow, indexHigh, indexDelta, indexCenter, i;

	if (filter_type == LPHPFILTER){
		// compute round(64*fHighpass/8000)
		indexLow = 4*fHighpass/125;
		if (indexLow & 0x01){
			indexLow = (indexLow >> 1) + 1;
		} else {
			indexLow = indexLow >> 1;
		}
		// compute round(64*fLowpass/8000)
		indexHigh = 4*fLowpass/125;
		if (indexHigh & 0x01){
			indexHigh = (indexHigh >> 1) + 1;
		} else {
			indexHigh = indexHigh >> 1;
		}
	}
	if (filter_type == BPFILTER){
		// compute round(64*bandwidth/8000)
		indexDelta = 4*bandwidth/125;
		if (indexDelta & 0x01){
			indexDelta = (indexDelta >> 1) + 1;
		} else {
			indexDelta = indexDelta >> 1;
		}
		if (indexDelta & 0x01){
			// odd number of bars
			// compute round(64*fc/8000)
			indexCenter = 4*fc/125;
			if (indexCenter & 0x01){
				indexCenter = (indexCenter >> 1) + 1;
			} else {
				indexCenter = indexCenter >> 1;
			}
			indexLow = indexCenter - (indexDelta >> 1);
			indexHigh = indexLow + indexDelta - 1;
		} else {
			// even number of bars
			// compute int(64*fc/8000)
			indexCenter = 2*fc/125;
			indexLow = indexCenter - (indexDelta >> 1) + 1;
			indexHigh = indexLow + indexDelta - 1;
		}
	}
	set_page_DOGS(page);
	set_column_DOGS(column);
	send_data_DOGS(0xFF);
	for (i = 0; i < indexLow; i++){
		send_data_DOGS(0x81);
	}
	for (i = indexLow; i <= indexHigh; i++){
		send_data_DOGS(0xBD);
	}
	for (i = indexHigh+1; i < 64;i++){
		send_data_DOGS(0x81);
	}
	send_data_DOGS(0xFF);
}

