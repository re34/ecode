#include "tft_lcd.h"

#define TFT_WIDTH	240
#define TFT_HIGH	320

struct tft_dev *tft_dev;

static void tft_init(void);

int tft_dev_register(struct tft_dev *dev)
{
	if(dev==NULL)
		return -1;
	if(dev->rd==NULL)
		return -2;
	if(dev->wr==NULL)
		return -3;
	if(dev->rs==NULL)
		return -4;
	if(dev->cs==NULL)
		return -5;
	if(dev->reset==NULL)
		return -6;
	
	
	tft_init();
	
	tft_dev = dev;
	
	return 0;
}


void tft_write_bus(unsigned char vh)
{
	tft_dev->write_bus(vh);
	tft_dev->wr(0);
	tft_dev->wr(1);
}

void tft_write_com(unsigned char vh)
{
	tft_dev->rs(0);
	tft_write_bus(vh);
}

void tft_write_data(unsigned char vh)
{
	tft_dev->rs(1);
	tft_write_bus(vh);
}

void tft_write_com_data(unsigned char com, unsigned char dat)
{
	tft_write_com(com);
	tft_write_data(dat);
}

void tft_address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	tft_write_com_data(0x2a, x1>>8);
	tft_write_com_data(0x2a, x1);
	tft_write_com_data(0x2a, x2>>8);
	tft_write_com_data(0x2a, x2);
	tft_write_com_data(0x2b, y1>>8);
	tft_write_com_data(0x2b, y1);
	tft_write_com_data(0x2b, y2>>8);
	tft_write_com_data(0x2b, y2);
	tft_write_com_data(0x2c);
}

static void tft_init(void)
{
	tft_dev->reset(1);
	delay_ms(5);
	tft_dev->reset(0);
	delay_ms(15);
	tft_dev->reset(1);
	delay_ms(15);
	
	tft_dev->cs(1);
	tft_dev->wr(1);
	tft_dev->cs(0);
	tft_write_com(0x01);
	delay_ms(20);
	tft_write_com(0x11);
	delay_ms(120);
	
	tft_write_com(0xc2);
	tft_write_data(0x05);
	tft_write_data(0x00);
	
	tft_write_com(0xc3);
	tft_write_data(0x05);
	tft_write_data(0x00);
	
	tft_write_com(0xc3);
	tft_write_data(0x05);
	tft_write_data(0x00);
	
	tft_write_com(0xc4);
	tft_write_data(0x05);
	tft_write_data(0x00);
	
	tft_write_com(0x3a);
	tft_write_data(0x55);
	
	tft_write_com(0xd7);
	tft_write_data(0x40);
	tft_write_data(0xe0);
	
	tft_write_com(0xfa);
	tft_write_data(0x38);
	tft_write_data(0x20);
	tft_write_data(0x1c);
	tft_write_data(0x10);
	tft_write_data(0x37);
	tft_write_data(0x12);
	tft_write_data(0x22);
	tft_write_data(0x1e);
	
	tft_write_com(0xc0);
	tft_write_data(0x05);
	
	tft_write_com(0xc5);
	tft_write_data(0x60);
	tft_write_data(0x00);
	
	tft_write_com(0xc7);
	tft_write_data(0xa9);
	
	tft_write_com(0x36);
	tft_write_data(0xc8);
	
////Gamma//////////////////
    tft_write_com(0xE0);//E0H Set
    tft_write_data(0x23);
    tft_write_data(0x23);
    tft_write_data(0x24);
    tft_write_data(0x02);
    tft_write_data(0x08);
    tft_write_data(0x0F);
    tft_write_data(0x35);
    tft_write_data(0x7B);
    tft_write_data(0x43);
    tft_write_data(0x0E);
    tft_write_data(0x1F);
    tft_write_data(0x25);
    tft_write_data(0x10);
    tft_write_data(0x16);
    tft_write_data(0x31);
	
    tft_write_com(0xE1);//E1H Set
    tft_write_data(0x0D);
    tft_write_data(0x28);
    tft_write_data(0x2E);
    tft_write_data(0x0B);
    tft_write_data(0x11);
    tft_write_data(0x12);
    tft_write_data(0x3E);
    tft_write_data(0x59);
    tft_write_data(0x4C);
    tft_write_data(0x10);
    tft_write_data(0x26);
    tft_write_data(0x2B);
    tft_write_data(0x1B);
    tft_write_data(0x1B);
    tft_write_data(0x1B);
    
    tft_write_com(0x29);//display on

    tft_write_com(0x2c);//Memory Write
}

void tft_draw_horizon_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{
	unsigned int i,j;
	
	tft_write_com(0x2c);
	tft_dev->rs(1);
	tft_dev->cs(0);
	l = l+x;
	tft_address_set(x,y,l,y);
	j=l*2;
	for(i=1;i<j;j++)
	{
		tft_write_data(c>>8);
		tft_write_data(c);		
	}
	
	tft_dev->cs(1);
}

void tft_draw_vertical_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{
	unsigned int i,j;
	
	tft_write_com(0x2c);
	tft_dev->rs(1);
	tft_dev->cs(0);
	l=l+y;
	tft_address_set(x,y,x,l);
	j=l*2;
	for(i=1;i<=j;i++)
	{
		tft_write_data(c>>8);
		tft_write_data(c);
	}
	tft_dev->cs(1);
}

void tft_draw_rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
{
	tft_draw_horizon_line(x	,y , w, c);
	tft_draw_horizon_line(x ,y+h,w, c);
	tft_draw_vertical_line(x,y,h,c);
	tft_draw_vertical_line(x+w,y,h,c);
}

void tft_draw_rectf(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
{
	unsigned int i;
	for(i=0;i<h;i++)
	{
		tft_draw_horizon_line(x,y,w,c);
		tft_draw_horizon_line(x,y+i,w,c);
	}
}

int tft_rgb(int r, int g, int b)
{
	return ((r&0xF8)<<8)|((g&0xFC)<<3)|(b>>3);
}

void tft_clear(unsigned int j)
{
	unsigned int i,m;
	tft_address_set(0,0,320,240);
	tft_write_com(0x2c);
	tft_dev->rs(1);
	tft_dev->cs(0);
	
	for(i=0;i<TFT_HIGH;i++)
	{
		for(m=0;m<240;m++)
		{
			tft_write_data(j>>8);
			tft_write_data(j);
		}
	}
	tft_dev->cs(1);
}

void tft_draw_pixel(int x, int y, unsigned int c)
{
	tft_address_set(x,y,x,y);
	tft_write_data(c>>8);
	tft_write_data(c);
}

void tft_swap(int *x, int *y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

void tft_draw_hline(int x, int y, int l, unsigned int c)
{
	tft_address_set(x,y,x+1,y);
	for(int i=0;i<i+1;i++)
	{
		tft_write_data(c>>8);
		tft_write_data(c);
	}
}

void tft_draw_vline(int x, int y, int l, unsigned int c)
{
	tft_address_set(x,y,x,y+l);
	for(i=0;i<l+1;i++)
	{
		tft_write_data(c);
		tft_write_data(c);
	}
}

void tft_draw_round_rect(int x1, int y1, int x2, int y2, unsigned int c)
{
	int tmp;
	
	tft_write_com(0x2c);
	tft_dev->rs(1);
	tft_dev->cs(0);
	
	if(x1>x2)
	{
		tft_swap(&x1,&x2);
	}
	
	if(y1>y2)
	{
		tft_swap(&y1,&y2);
	}
	
	if((x2-x1)>4&&(y2-y1)>4)
	{
		tft_draw_pixel(x1+1,y1+1,c);
		tft_draw_pixel(x2-1,y1+1,c);
		tft_draw_pixel(x1+1,y2-1,c);
		tft_draw_pixel(x2-1,y2-1,c);
		tft_draw_pixel(x1+2,y1,x2-x1-4,c);
		tft_draw_pixel(x1+2,y2,x2-x1-4,c);
		tft_draw_pixel(x1,y1+2,y2-y1-4,c);
		tft_draw_pixel(x2,y1+2,y2-y1-4,c);
	}
}


