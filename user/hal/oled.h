#ifndef __OLED_H__
#define __OLED_H__
#include "ecode.h"


struct oled_dev{
	int (*init)(void);
	void (*reset)(unsigned char status);
	void (*sclk)(unsigned char status);
	void (*dc)(unsigned char status);
	void (*cs)(unsigned char status);
	void (*sdin)(unsigned char status);
};


#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

int oled_register(struct oled_dev *dev);
void oled_set_pos(unsigned char x, unsigned char y);
void oled_display_on();
void oled_display_off();
void oled_clear(void);
void oled_show_char(UInt8 x, UInt8 y, UInt8 chr);
UInt32 oled_pow(UInt8 m,UInt8 n);
void oled_show_num(UInt8 x, UInt8 y, UInt32 num, UInt8 len, UInt8 size);
void oled_show_string(UInt8 x, UInt8 y, UInt8 *str);
void oled_show_chinese(UInt8 x, UInt8 y, UInt8 no);
void oled_draw_bmp(UInt8 x0, UInt8 y0, UInt8 x1, UInt8 y1, UInt8 *bmp);
#endif