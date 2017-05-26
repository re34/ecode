#ifndef __TFT_LCD_H__
#define __TFT_LCD_H__

struct tft_dev{
	void (*rd)(unsigned char);
	void (*wr)(unsigned char);
	void (*rs)(unsigned char);
	void (*cs)(unsigned char);
	void (*reset)(unsigned char);
	int (*write_bus)(int data);
};

int tft_dev_register(struct tft_dev *dev);
void tft_draw_horizon_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void tft_draw_vertical_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void tft_draw_rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void tft_draw_rectf(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
int tft_rgb(int r, int g, int b);
void tft_clear(unsigned int j);
void tft_draw_pixel(int x, int y, unsigned int c);
void tft_swap(int *x, int *y);
void tft_draw_hline(int x, int y, int l, unsigned int c);
void tft_draw_vline(int x, int y, int l, unsigned int c);
void tft_draw_round_rect(int x1, int y1, int x2, int y2, unsigned int c);

#endif