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
 *  DOGS.c
 * 
 * Description:
 *  This file implements the functions concerning the DOGS display
 *******************************************************************************/

#include "xc.h"
#include "constants.h"
#include <libpic30.h>
#include "DOGS.h"
#include "SPI.h"
#include "font5x7.h"
#include "GenericTypeDefs.h"
#include "globals.h"
#include <libpic30.h>
#include <stdio.h>

#define DOGS_N_INIT 13

unsigned int actual_column, actual_page;
unsigned int x_cursor, y_cursor;


void init_DOGS(void){
	int i;
	const char init_data[DOGS_N_INIT] = {
        0x40, //Set the display startline number: SL=0
        0xA0, //Set SEG direction: normal
        0xC8, //Set COM direction: mirror
        0xA4, //Set All Pixel On: show SRAM content
        0xA6, //Set Inverse Display: show normal SRAM content
        0xA2, //Set LCD Bias Ratio: BR=1/9
        0x2F, //Set Power Control: Booster ON, Regulator ON, Follower ON
        0x27, //Set VLCD Resistor Ratio: 7
        0x81, 0x0A, //Adjust contrast of LCD panel: 0x0A
        0xFA, 0x90, //Set Adv. Program Control 0: Temp. comp. -0,11%/°C, Column wrap around OFF, Page wrap around OFF
        0xAF //enable Display
    };

	CS0 = 1;			// Set chip select (CS0) to 1 = not selected
	CS0_TRIS = 0;		// Set port CS0 to output
	CD = 0;				// Set CD to 0  = command mode 
	CD_TRIS = 0;		// Set port CD to output
	RST = 0;			// Apply Reset
	RST_TRIS = 0;		// Set port RST to output
	__delay_ms(10);
	RST = 1;			// Remove Reset
	__delay_ms(10);

	for (i = 0; i < DOGS_N_INIT; i++){
		send_cmd_DOGS(init_data[i]);
	}
}

void set_column_DOGS(unsigned int c){
	unsigned int cl, ch;

	if(c < 102){
		actual_column = c;
		c += 30;
		cl = c & 0x0F;
		ch = (c & 0xF0) >> 4;
		send_cmd_DOGS(cl);
		send_cmd_DOGS(ch | 0x10);
	}
}

void set_page_DOGS(unsigned int p){

	if(p<8){
		actual_page = p;
		p = p & 0x0F;
		send_cmd_DOGS(p + 0xB0);
	}	
}

void send_data_DOGS(char data){
	unsigned int dummy;

	CS0 = 0;						// select chip
	CD = 1;							// data mode
	dummy = SPI1BUF;				// Clear SPIx Receive Buffer Full Status bit (SPIRBF)
	send_SPI(data);					// send data, SPITBF will be set
	while(!SPI1STATbits.SPIRBF);	// wait until transfer complete (SPIRBF set)
	CS0 = 1;						// deselect chip and reset bus cycle
}

void send_cmd_DOGS(char cmd){
	unsigned int dummy;

	CS0 = 0;						// select chip
	CD = 0;							// command mode
	dummy = SPI1BUF;				// Clear SPIx Receive Buffer Full Status bit (SPIRBF)
	send_SPI(cmd);					// send data, SPITBF will be set
	while(!SPI1STATbits.SPIRBF);	// wait until transfer complete (SPIRBF set)
	CS0 = 1;						// deselect chip and reset bus cycle
}

void clear_DOGS(void){
	unsigned int i,j;

	for (i=0; i<8; i++){
		set_page_DOGS(i);
		for (j=0; j<102; j++){
			set_column_DOGS(j);
			send_data_DOGS(0x00);
		}
	}
}

void put_char_DOGS(char ch){
	unsigned int i;
	
	ch -= 0x20;
	for (i=0;i<5;i++){
		send_data_DOGS(FONT_5x7[(BYTE)ch][i]);
	}
	set_column_DOGS(actual_column + 6);
}

void set_xy_DOGS(unsigned int x, unsigned int y){
	x_cursor = x;
	y_cursor = y;
}

void put_char_xy_DOGS(char ch){
	unsigned int i;
	int column, lower_page, upper_page, offset;

	ch -= 0x20;
	column = x_cursor;
	lower_page = y_cursor >> 3;
	upper_page = lower_page - 1;
	offset = 7 - (y_cursor & 7);
	set_column_DOGS(column);
	for (i=0; i<5; i++){
		set_page_DOGS(lower_page);
		send_data_DOGS(FONT_5x7[(BYTE)ch][i]>>offset);
	}
	send_data_DOGS(0x00>>offset);
	if(offset){
		set_column_DOGS(column);
		for (i=0; i<5; i++){
			set_page_DOGS(upper_page);
			send_data_DOGS(FONT_5x7[(BYTE)ch][i]<<(8-offset));
		}
		send_data_DOGS(0x00<<(8-offset));
	}
	x_cursor += 6;			
}

void write_int_DOGS(int value){
	char s[7];
	int i,n;

	n = sprintf(s, "%6d",value);
	for (i=0; i<n; i++){
		put_char_xy_DOGS(s[i]);
	}
}	

void puts_DOGS(char *s){
	int i = 0;

	while (s[i]){
		put_char_xy_DOGS(s[i++]);
	}
}


void put_char_xy_DOGS_inverted(char ch){
	unsigned int i;
	int column, lower_page, upper_page, offset;

	ch -= 0x20;
	column = x_cursor;
	lower_page = y_cursor >> 3;
	upper_page = lower_page - 1;
	offset = 7 - (y_cursor & 7);
	set_column_DOGS(column);
	set_page_DOGS(lower_page);
	for (i=0; i<5; i++){
		send_data_DOGS((FONT_5x7[(BYTE)ch][i]>>offset)^0xFF);
	}
	send_data_DOGS((0X00>>offset)^0xFF);
	if(offset){
		set_column_DOGS(column);
		set_page_DOGS(upper_page);
		for (i=0; i<5; i++){
			send_data_DOGS((FONT_5x7[(BYTE)ch][i]<<(8-offset))^0xFF);
		}
		send_data_DOGS((0x00<<(8-offset))^0xFF);
	}
	x_cursor += 6;			
}

void puts_DOGS_inverted(char *s){
	int i = 0;

	while (s[i]){
		put_char_xy_DOGS_inverted(s[i++]);
	}
}

void clearPartScreen(int page, int column, int nPage, int nColumn){
	int i, j;

	for (i=page; i<page+nPage; i++){
		set_page_DOGS(i);
		set_column_DOGS(column);
		for (j=column; j<column+nColumn; j++){
			send_data_DOGS(0x00);
		}
	}
}

