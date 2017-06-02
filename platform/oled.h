#ifndef __OLED_H__
#define __OLED_H__
#include "includes.h"
#include "font.h"


#define FONT32      32
#define FONT16	16
#define FONT12	12

struct oled_dev{
	int (*init)(void);
	void (*reset)(unsigned char status);
	void (*sclk)(unsigned char status);
	void (*dc)(unsigned char status);
	void (*cs)(unsigned char status);
	void (*sdin)(unsigned char status);
};


int oled_register(struct oled_dev *dev);
void oled_disable_refresh(void);
void oled_enable_refresh(void);
void oled_refresh_gram(void);
void oled_draw_pixel(UInt16 x, UInt16 y, UInt8 color);
UInt8 oled_get_pixel(UInt16 x, UInt16 y);
void    oled_draw_point(UInt16 x, UInt16 y, UInt8 pad);
void oled_fill(UInt8 x1, UInt8 y1, UInt8 x2, UInt8 y2, UInt8 pad);
void oled_draw_line(UInt16 x1, UInt16 y1, UInt16 x2, UInt16 y2, UInt8 color);
void oled_draw_points(UInt16 *x, UInt16 *y, UInt16 size, UInt8 color);
void oled_draw_rect(UInt16 x, UInt16 y, UInt16 width, UInt16 height, UInt8 color);
void oled_draw_circle(UInt16 x, UInt16 y, UInt16 _usRadius, uint8_t _ucColor);
void oled_draw_bmp(UInt16 _usX, UInt16 _usY, UInt16 _usHeight, UInt16 _usWidth, uint8_t *_ptr);
void oled_draw_char(UInt8 x, UInt8 y, UInt8 chr, UInt8 font, UInt8 color);
void oled_draw_num(UInt8 x, UInt8 y, UInt32 num, UInt8 len, UInt8 font);
void oled_draw_string(char *str, UInt8 x, UInt8 y, UInt8 font);
void oled_clear(UInt8 data);

#endif