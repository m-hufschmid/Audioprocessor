#ifndef _GUI_H
#define _GUI_H

#define BPFILTER 0
#define LPHPFILTER 1

#define FILTER_PAGE 1
#define NOTCH_PAGE 2
#define FFT_TOP_PAGE 5
#define OVERFLOW_PAGE 7
#define DISPLAY_PARAMETERS_PAGE 4

void build_menu();
void build_GUI();
void update_GUI();
void plotFilterBar(int, int);

#endif
