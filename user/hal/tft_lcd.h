#ifndef __TFT_LCD_H__
#define __TFT_LCD_H__
#include "ecode.h"

struct tft_dev{
	void (*rd)(unsigned char);
	void (*wr)(unsigned char);
	void (*rs)(unsigned char);
	void (*cs)(unsigned char);
	void (*reset)(unsigned char);
	int (*write_bus)(int data);
};

int tft_dev_register(struct tft_dev *dev);
void tft_clear(UInt16 c);

#endif