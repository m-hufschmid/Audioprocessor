#ifndef _MENU_H_
#define _MENU_H_

#include <stdint.h>

void menu(void);
void buildMenu(int menuTitle);
int updateMenu(int menuTitle);
void gain_input_to_string(char *s);
void gain_headphone_to_string(char *s);
void setInputGain(int);
void setHeadphoneGain(int);
void plotGainBar(int, int);

#endif
