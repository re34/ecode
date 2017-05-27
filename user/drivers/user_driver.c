#include "user_driver.h"
#include "ecode.h"
#include "tft_lcd.h"

static struct tft_dev tft_dev;


#define LCD_RD   A0
#define LCD_WR   A1     
#define LCD_RS   A2        
#define LCD_CS   A3       
#define LCD_RESET A4

const unsigned char pins[]={D8,D9,D2,D3,D4,D5,D6,D7};

static void tft_lcd_write_bus(int vh);
static void tft_rs(unsigned char status);
static void tft_rd(unsigned char status);
static void tft_wr(unsigned char status);
static void tft_cs(unsigned char status);
static void tft_reset(unsigned char status);
static void tft_lcd_init();

void user_driver_init()
{
	
	tft_lcd_init();
}


static void tft_lcd_init()
{
    int i;
    LOG_DEBUG("TFT INIT....");
    bsp_pin_digital_out_init(A0, PIN_PULLUP);
    bsp_pin_digital_out_init(A1, PIN_PULLUP);
    bsp_pin_digital_out_init(A2, PIN_PULLUP);
    bsp_pin_digital_out_init(A3, PIN_PULLUP);
    bsp_pin_digital_out_init(A4, PIN_PULLUP);
	bsp_pin_set(A0);
	bsp_pin_set(A1);
	bsp_pin_set(A2);
	bsp_pin_set(A3);
	bsp_pin_set(A4);
    LOG_DEBUG("control pin inited");
    for(i=0;i<=7;i++)
    {
        bsp_pin_digital_out_init(pins[i], PIN_PULLUP);
    }
    LOG_DEBUG("DATA PIN inited");
    tft_dev.rs = tft_rs;
    tft_dev.rd = tft_rd;
    tft_dev.wr = tft_wr;
    tft_dev.cs = tft_cs;
    tft_dev.reset = tft_reset;
    tft_dev.write_bus = tft_lcd_write_bus;
    tft_dev_register(&tft_dev);
    tft_clear( tft_rgb(255,0,0));
    tft_clear( tft_rgb(255,255,0));
    tft_clear( tft_rgb(255,0,255));
    tft_clear( tft_rgb(0,255,255));
    tft_clear( tft_rgb(0,255,0));
}




static void tft_lcd_write_bus(int vh)
{
  unsigned int i,temp,data; 
  data=vh;
  for(i=0;i<=7;i++)
  {
    temp=(data&0x01);
    if(temp)
      bsp_pin_set(pins[i]);
    else
      bsp_pin_reset(pins[i]);
    data=data>>1;
  }	     
    
}

static void tft_rs(unsigned char status)
{
    if(status)
        bsp_pin_set(LCD_RS);
    else
        bsp_pin_reset(LCD_RS);
}
static void tft_rd(unsigned char status)
{
    if(status)
        bsp_pin_set(LCD_RD);
    else
        bsp_pin_reset(LCD_RD);
}
static void tft_wr(unsigned char status)
{
    if(status)
        bsp_pin_set(LCD_WR);
    else
        bsp_pin_reset(LCD_WR);
}
static void tft_cs(unsigned char status)
{
    if(status)
        bsp_pin_set(LCD_CS);
    else
        bsp_pin_reset(LCD_CS);
}
static void tft_reset(unsigned char status)
{
    if(status)
        bsp_pin_set(LCD_RESET);
    else
        bsp_pin_reset(LCD_RESET);
}
