#include "user_oled.h"
#include "ecode.h"
#include "oled.h"
#include "stm_pin.h"


#define CS   A0
#define DC    A1
#define RESET   A2
#define SDIN    A3
#define SCLK    A4

static struct oled_dev oled_dev;

static int oled_init();
static void oled_cs(unsigned char status);
static void oled_dc(unsigned char status);
static void oled_reset(unsigned char status);
static void oled_sclk(unsigned char status);
static void oled_sdin(unsigned char status);

void user_oled_init()
{
    oled_dev.init = oled_init;
    oled_dev.cs = oled_cs;
    oled_dev.dc = oled_dc;
    oled_dev.reset = oled_reset;
    oled_dev.sclk = oled_sclk;
    oled_dev.sdin = oled_sdin;
    
    oled_register(&oled_dev);

    oled_draw_string("Ecode Board",10,10,FONT16);
}

static int oled_init()
{
    pin_mode(CS, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    pin_mode(DC, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    pin_mode(RESET, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    pin_mode(SCLK, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    pin_mode(SDIN, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    LOG_DEBUG("oled pin inited!");
    return 0;
}

static void oled_cs(unsigned char status)
{
    if(status==PIN_SET)
        pin_write(CS, PIN_HIGH);
    else
        pin_write(CS, PIN_LOW);
}

static void oled_dc(unsigned char status)
{
    if(status==PIN_SET)
        pin_write(DC, PIN_HIGH);
    else
        pin_write(DC, PIN_LOW);
}

static void oled_reset(unsigned char status)
{
    if(status==PIN_SET)
        pin_write(RESET, PIN_HIGH);
    else
        pin_write(RESET, PIN_LOW);
}

static void oled_sclk(unsigned char status)
{
    if(status==PIN_SET)
        pin_write(SCLK, PIN_HIGH);
    else
        pin_write(SCLK, PIN_LOW);
}

static void oled_sdin(unsigned char status)
{
    if(status==PIN_SET)
        pin_write(SDIN, PIN_HIGH);
    else
        pin_write(SDIN, PIN_LOW);
}