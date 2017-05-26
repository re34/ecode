#include "display.h"
#include "ecode.h"
#include "led.h"
#include "includes.h"
#include "tft_lcd.h"

struct timeout led_timeout;

static struct tft_dev tft_dev;
void tft_lcd_init();

void display_callback(void)
{
    led_toggle(LED1);
    tick_attach_timeout(&led_timeout, display_callback,500);
    
}

void display_task(void *args)
{
    LOG_DEBUG("display_task running...");
    led_init();
    wait_system_on();
    tick_attach_timeout(&led_timeout, display_callback,500);
    //pwm_period_ms(PWM1,60);
    //pwm_write(PWM1, 0.5);
    
    tft_lcd_init();
#if RTOS_EN==1
    while(1)
    {
        vTaskDelay(500);
    }
#endif
}


void display_init(void)
{
#if RTOS_EN==1
    xTaskCreate(display_task,
                "display_task",
                1024,
                NULL,
                3,
                NULL);
#endif
}

#define LCD_RD   A0
#define LCD_WR   A1     
#define LCD_RS   A2        
#define LCD_CS   A3       
#define LCD_RESET A4
const unsigned char pins[]={D2,D3,D4,D5,D6,D7,D8,D9};
static void tft_lcd_write_bus(int vh);
static void tft_rs(unsigned char status);
static void tft_rd(unsigned char status);
static void tft_wr(unsigned char status);
static void tft_cs(unsigned char status);
static void tft_reset(unsigned char status);
void tft_lcd_init()
{
    int i;
    LOG_DEBUG("TFT INIT....");
    bsp_pin_digital_out_init(A0, PIN_PULLUP);
    bsp_pin_digital_out_init(A1, PIN_PULLUP);
    bsp_pin_digital_out_init(A2, PIN_PULLUP);
    bsp_pin_digital_out_init(A3, PIN_PULLUP);
    bsp_pin_digital_out_init(A4, PIN_PULLUP);
    LOG_DEBUG("control pin inited");
    for(i=6;i<=7;i++)
    {
        bsp_pin_digital_out_init(pins[i], PIN_PULLUP);
    }	
    LOG_DEBUG("8-9,inited");
    for(i=0;i<=5;i++)
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
    LOG_DEBUG("TFT inited!!!");
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
  for(i=6;i<=7;i++)
  {
    temp=(data&0x01);
    if(temp)
      bsp_pin_set(pins[i]);
    else
      bsp_pin_reset(pins[i]);
    data=data>>1;
  }	
  for(i=0;i<=5;i++)
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


