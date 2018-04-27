#ifndef DOGS_H
#define DOGS_H

#define CD 			LATGbits.LATG7
#define CD_TRIS 	TRISGbits.TRISG7	
#define RST 		LATGbits.LATG9
#define RST_TRIS 	TRISGbits.TRISG9
#define CS0 		LATEbits.LATE7
#define CS0_TRIS 	TRISEbits.TRISE7

void init_DOGS(void);
void set_column_DOGS(unsigned int);
void set_page_DOGS(unsigned int);
void send_data_DOGS(char);
void send_cmd_DOGS(char);
void clear_DOGS(void);
void put_char_DOGS(char ch);
void set_xy_DOGS(unsigned int x, unsigned int y);
void put_char_xy_DOGS(char ch);
void write_int_DOGS(int value);
void puts_DOGS(char *s);
void puts_DOGS_inverted(char *s);
void put_char_xy_DOGS_inverted(char ch);
void clearPartScreen(int page, int column, int nPage, int nColumn);

#endif
