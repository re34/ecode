#ifndef __OLED_H__
#define __OLED_H__
#include "types.h"
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
void oled_draw_pixel(e_uint16_t x, e_uint16_t y, e_uint8_t color);
e_uint8_t oled_get_pixel(e_uint16_t x, e_uint16_t y);
void    oled_draw_point(e_uint16_t x, e_uint16_t y, e_uint8_t pad);
void oled_fill(e_uint8_t x1, e_uint8_t y1, e_uint8_t x2, e_uint8_t y2, e_uint8_t pad);
void oled_draw_line(e_uint16_t x1, e_uint16_t y1, e_uint16_t x2, e_uint16_t y2, e_uint8_t color);
void oled_draw_points(e_uint16_t *x, e_uint16_t *y, e_uint16_t size, e_uint8_t color);
void oled_draw_rect(e_uint16_t x, e_uint16_t y, e_uint16_t width, e_uint16_t height, e_uint8_t color);
void oled_draw_circle(e_uint16_t x, e_uint16_t y, e_uint16_t _usRadius, e_uint8_t _ucColor);
void oled_draw_bmp(e_uint16_t _usX, e_uint16_t _usY, e_uint16_t _usHeight, e_uint16_t _usWidth, e_uint8_t *_ptr);
void oled_draw_char(e_uint8_t x, e_uint8_t y, e_uint8_t chr, e_uint8_t font, e_uint8_t color);
void oled_draw_num(e_uint8_t x, e_uint8_t y, e_uint32_t num, e_uint8_t len, e_uint8_t font);
void oled_draw_string(char *str, e_uint8_t x, e_uint8_t y, e_uint8_t font);
void oled_clear(e_uint8_t data);

#endif