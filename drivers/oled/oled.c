#include "oled.h"
#include "font.h"
#include <string.h>
#include "ecode.h"



#define    OLED_OFF    0xAE
#define    OLED_ON    0xAF
#define    OLED_SET_LINEXX    0x40
#define    OLED_SET_PAGEXX    0xB0
#define    OLED_SET_UCOLXX    0x10
#define    OLED_SET_DCOLXX    0x00
//#define    OLED_READ_STATUS
#define    OLED_ADC_NORMAL    0xA0
#define    OLED_ADC_REVERSE    0xA1
#define    OLED_DISPLAY_NORMAL    0xA6
#define    OLED_DISPLAY_REVERSE    0xA7
#define    OLED_DISPLAY_NONE    0xA4
#define    OLED_DISPLAY_ALL    0xA5
#define    OLED_SET_BIAS9    0xA2
#define    OLED_SET_BIAS7    0xA3
#define    OLED_COL_INC        0xE0
#define    OLED_END        0xEE
#define    OLED_RESET        0xE2
#define    OLED_COM_DIR0        0xC0
#define    OLED_COM_DIR1        0xC8
#define    OLED_SET_PWR        0x2F
#define    OLED_NOP    0xE3


#define    OLED_COLS    128
#define    OLED_LINES    64
#define    OLED_PAGES    8
#define    OLED_WIDTH    128
#define    OLED_HIGH        64

#define OLED_GRAM_WIDTH    OLED_WIDTH
#define OLED_GRAM_HIGH    (OLED_HIGH/8)
#define OLED_GRAM_SIZE        (OLED_GRAM_HIGH*OLED_GRAM_WIDTH)

#define oled_write_cmd(cmd)    BspOledWriteCmd(cmd)
#define oled_write_data(data)    BspOledWriteData(data)

e_uint8_t OLED_GRAM[OLED_GRAM_SIZE];

e_uint8_t isOledRefreshEn = 0;

struct oled_dev *oled_dev;

static void _oled_init();


static void _oled_init(void);

int oled_register(struct oled_dev *dev)
{
	if(dev==NULL)
		return -1;
	if(dev->init()<0)
		return -2;
	oled_dev = dev;
	_oled_init();
	LOG_DEBUG("oled inited!!!");
	return 0;
}


void oled_write_cmd(unsigned char cmd)
{
    unsigned char i;
    
    oled_dev->dc(PIN_LOW);
    oled_dev->cs(PIN_LOW);
    
    for(i=0;i<8;i++)
    {
        oled_dev->sclk(PIN_LOW);
        if(cmd&0x80)
            oled_dev->sdin(PIN_HIGH);
        else
            oled_dev->sdin(PIN_LOW);
        oled_dev->sclk(PIN_HIGH);
        cmd<<=1;
    }
    oled_dev->cs(PIN_HIGH);
    oled_dev->dc(PIN_HIGH); 
}

void oled_write_data(unsigned char data)
{
    unsigned char i;
    
    oled_dev->dc(PIN_HIGH);
    oled_dev->cs(PIN_LOW);
    
    for(i=0;i<8;i++)
    {
        oled_dev->sclk(PIN_LOW);
        if(data&0x80)
            oled_dev->sdin(PIN_HIGH);
        else
            oled_dev->sdin(PIN_LOW);
        oled_dev->sclk(PIN_HIGH);
        data<<=1;
    }
    oled_dev->cs(PIN_HIGH);
    oled_dev->dc(PIN_HIGH);
}



void oled_disable_refresh(void)
{
    isOledRefreshEn = 0;
}

void oled_enable_refresh(void)
{
    isOledRefreshEn = 1;
    oled_refresh_gram();
}
void oled_page_clear(e_uint8_t page, e_uint8_t data)
{
    e_uint8_t page_addr;
    e_uint8_t col;

    page_addr = OLED_SET_PAGEXX+page;
    oled_write_cmd(page_addr);
    oled_write_cmd(OLED_SET_UCOLXX);
    oled_write_cmd(OLED_SET_DCOLXX);
    for(col=0;col<OLED_COLS;col++)
    {
        oled_write_data(data);
    }
}

void oled_set_page(e_uint8_t page)
{
    oled_write_cmd(OLED_SET_PAGEXX|page);
}

void oled_set_column(e_uint8_t col)
{
    e_uint8_t tmp;
    tmp=col&0x0F;
    oled_write_cmd(OLED_SET_DCOLXX|tmp);
    tmp = col>>4;
    oled_write_cmd(OLED_SET_UCOLXX|tmp);

}

void oled_draw_page(e_uint8_t page, e_uint8_t col, e_uint8_t page_data){
    oled_set_page(page);
    oled_set_column(col);
    oled_write_data(page_data);
    
}

void oled_refresh_gram(void)
{
    e_uint8_t i, j;
    e_uint8_t *pBuf=OLED_GRAM;

    for(i=0;i<OLED_GRAM_HIGH;i++)
    {
        oled_set_page(i);
        oled_set_column(0);
        for(j=0;j<OLED_GRAM_WIDTH;j++)
        {
            oled_write_data(*pBuf);
            pBuf++;
        }
    }
}

void oled_draw_pixel(e_uint16_t x, e_uint16_t y, e_uint8_t color)
{
    e_uint8_t page =0;
    e_uint8_t page_data;

    if((x>=OLED_WIDTH)||(y>=OLED_HIGH))
        return;
    page = y/8;
    
    page_data = OLED_GRAM[page*OLED_GRAM_WIDTH+x];
    if(color)
        page_data |= (1<<(y%8));
    else
        page_data &=~(1<<(y%8));
    OLED_GRAM[page*OLED_GRAM_WIDTH+x]=page_data;

    if(isOledRefreshEn==1)
    {
        oled_draw_page(page, x, page_data);
    }
}

e_uint8_t oled_get_pixel(e_uint16_t x, e_uint16_t y)
{
    e_uint8_t page =0;
    e_uint8_t page_data;

    if((x>=OLED_WIDTH)||(y>=OLED_HIGH))
        return 0;
    page = y/8;
    
    page_data = OLED_GRAM[page*OLED_GRAM_WIDTH+x];
    if(page_data &(1<<(y%8)))
        return 1;
    else
        return 0;
}

void    oled_draw_point(e_uint16_t x, e_uint16_t y, e_uint8_t pad){
    e_uint8_t page =0;
    e_uint8_t page_data;

    if((x>=OLED_WIDTH)||(y>=OLED_HIGH))
        return;
    page = y/8;
    
    page_data = OLED_GRAM[page*OLED_GRAM_WIDTH+x];
    if(pad)
        page_data |= (1<<(y%8));
    else
        page_data &=~(1<<(y%8));
    OLED_GRAM[page*OLED_GRAM_WIDTH+x]=page_data;
}

void oled_fill(e_uint8_t x1, e_uint8_t y1, e_uint8_t x2, e_uint8_t y2, e_uint8_t pad)
{
    e_uint8_t x, y;

    for(x=x1;x<x2; x++)
    {
        for(y=y1;y<y2;y++)
        {
            //oled_draw_point(x, y, pad);
            oled_draw_pixel(x, y, pad);
        }
    }
}


void oled_draw_line(e_uint16_t x1, e_uint16_t y1, e_uint16_t x2, e_uint16_t y2, e_uint8_t color){
    e_int32_t dx, dy;
    e_int32_t tx, ty;
    e_int32_t inc1, inc2;
    e_int32_t d, iTag;
    e_int32_t x, y;

    oled_draw_pixel(x1, y1, color);

    if((x1==x2)&&(y1==y2))
        return;
    iTag = 0;

    if(x2>=x1)
        dx=x2-x1;
    else
        dx = x1-x2;

    if(y2>=y1)
        dy = y2-y1;
    else
        dy = y1-y2;

    if(dx<dy)
    {
        e_uint16_t temp;
        iTag = 1;
        temp = x1;
        x1 = y1;
        y1 = temp;

        temp = x2;
        x2 = y2;
        y2 = temp;

        temp = dx;
        dx = dy;
        dy = temp;
    }

    tx = x2>x1?1:-1;
    ty = y2>y1?1:-1;

    x = x1;
    y = y1;

    inc1 = 2*dy;
    inc2 = 2*(dy-dx);

    d = inc1-dx;
    while(x!=x2)
    {
        if(d<0)
            d+=inc1;
        else
        {
            y+=ty;
            d+=inc2;
        }
        if(iTag)
            oled_draw_pixel(y, x, color);
        else
            oled_draw_pixel(x, y, color);
        x+=tx;
    }

}

void oled_draw_points(e_uint16_t *x, e_uint16_t *y, e_uint16_t size, e_uint8_t color)
{
    e_uint16_t i;

    for(i=0;i<size-1;i++)
    {
        oled_draw_line(x[i], y[i], x[i+1], y[i+1], color);
    }
}


void oled_draw_rect(e_uint16_t x, e_uint16_t y, e_uint16_t width, e_uint16_t height, e_uint8_t color)
{
    oled_draw_line(x, y, x+width-1, y, color);
    oled_draw_line(x, y, x, y+height-1, color);
    oled_draw_line(x, y+height-1, x+width-1, y+height-1, color);
    oled_draw_line(x+width-1, y, x+width-1, y+height-1, color);
}
void oled_draw_circle(e_uint16_t x, e_uint16_t y, e_uint16_t _usRadius, uint8_t _ucColor)
{
    e_int32_t D;            /* Decision Variable */
    e_uint32_t CurX;        /* 当前 X 值 */
    e_uint32_t CurY;        /* 当前 Y 值 */

    D = 3 - (_usRadius << 1);
    CurX = 0;
    CurY = _usRadius;

    while (CurX <= CurY)
    {
        oled_draw_pixel(x + CurX, y + CurY, _ucColor);
        oled_draw_pixel(x + CurX, y - CurY, _ucColor);
        oled_draw_pixel(x - CurX, y + CurY, _ucColor);
        oled_draw_pixel(x - CurX, y - CurY, _ucColor);
        oled_draw_pixel(x + CurY, y + CurX, _ucColor);
        oled_draw_pixel(x + CurY, y - CurX, _ucColor);
        oled_draw_pixel(x - CurY, y + CurX, _ucColor);
        oled_draw_pixel(x - CurY, y - CurX, _ucColor);

        if (D < 0)
        {
            D += (CurX << 2) + 6;
        }
        else
        {
            D += ((CurX - CurY) << 2) + 10;
            CurY--;
        }
        CurX++;
    }
}

void oled_draw_bmp(e_uint16_t _usX, e_uint16_t _usY, e_uint16_t _usHeight, e_uint16_t _usWidth, uint8_t *_ptr)
{
    e_uint16_t x, y;

    for (x = 0; x < _usWidth; x++)
    {
        for (y = 0; y < _usHeight; y++)
        {
            oled_draw_pixel(_usX + x, _usY + y, *_ptr);
        }
    }
}


void oled_draw_char(e_uint8_t x, e_uint8_t y, e_uint8_t chr, e_uint8_t font, e_uint8_t color)
{
    e_uint8_t *pC;

    e_uint8_t font_size;

    e_uint8_t i, j;
    e_uint8_t x0=x;
    e_uint8_t y0 = y;
    e_uint8_t y_base = y;
    e_uint8_t temp;

    font_size = (font/8+((font%8)?1:0))*(font/2);

    

    switch(font)
    {
    case FONT16:
        pC = (e_uint8_t *)&ascii16[font_size*chr];
        break;
    case FONT12:
        pC = (e_uint8_t *)&ascii12[font_size*chr];
        break;
    }
    
    for(i=0;i<font_size;i++)
    {
        temp = *pC;
        for(j=0;j<8;j++)
        {
            if(temp&0x01)
                oled_draw_pixel(x, y, color);
            else
                oled_draw_pixel(x, y, !color);
            temp>>=1;
            y++;
            if(y-y_base>=font)
                break;
        }
        x++;
        if(x-x0>=font/2)
        {
            x=x0;
            y0+=8;
        }
        y=y0;
        pC++;
    }
    
}

e_uint32_t oled_pow(e_uint8_t m, e_uint8_t n)
{
    e_uint32_t result = 1;
    while(n--)
        result*=m;
    return result;
}

void oled_draw_num(e_uint8_t x, e_uint8_t y, e_uint32_t num, e_uint8_t len, e_uint8_t font)
{
    e_uint8_t temp, i;
    e_uint8_t enshow = 0;

    for(i=0;i<len;i++)
    {
        temp = (num/oled_pow(10, len-i-1))%10;
        if((enshow==0)&&(i<(len-1)))
        {
            if(temp==0)
            {
                oled_draw_char(x+(font/2)*i, y, ' ', font, 1);
            }
            else
                enshow = 1;
        }
        oled_draw_char(x+(font/2)*i, y, temp+'0', font, 1);
    }
    
}


void oled_draw_string(char *str, e_uint8_t x, e_uint8_t y, e_uint8_t font)
{
    e_uint16_t len = strlen(str);

    e_uint8_t font_width;

    font_width = font/2;
    while(*str!='\0')
    {
        oled_draw_char(x, y, *str, font,1);
        x+=font_width;
        str++;
    }
}



void oled_clear(e_uint8_t data){

    e_uint8_t page;
    e_uint8_t col;
    e_uint8_t *pGram = OLED_GRAM;
    

    for(page=0;page<8;page++){
        oled_set_page(page);
        oled_set_column(0);
        for(col=0;col<OLED_COLS;col++){
            oled_write_data(data);
            *pGram++=0;
        }
    }
}

static void _oled_init(void)
{

    oled_dev->reset(PIN_HIGH);
    oled_dev->cs(PIN_HIGH);
    oled_dev->dc(PIN_HIGH);
    oled_dev->sdin(PIN_HIGH);
    oled_dev->sclk(PIN_HIGH);
    
    oled_dev->reset(PIN_HIGH);
    delay_ms(100);
    oled_dev->reset(PIN_LOW);
    delay_ms(200);
    oled_dev->reset(PIN_HIGH);
    
    oled_write_cmd(0xAE);//--turn off oled panel
    oled_write_cmd(0x00);//---set low column address
    oled_write_cmd(0x10);//---set high column address
    oled_write_cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    oled_write_cmd(0x81);//--set contrast control register
    oled_write_cmd(0xCF); // Set SEG Output Current Brightness
    oled_write_cmd(0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    oled_write_cmd(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    oled_write_cmd(0xA6);//--set normal display
    oled_write_cmd(0xA8);//--set multiplex ratio(1 to 64)
    oled_write_cmd(0x3f);//--1/64 duty
    oled_write_cmd(0xD3);//-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
    oled_write_cmd(0x00);//-not offset
    oled_write_cmd(0xd5);//--set display clock divide ratio/oscillator frequency
    oled_write_cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    oled_write_cmd(0xD9);//--set pre-charge period
    oled_write_cmd(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    oled_write_cmd(0xDA);//--set com pins hardware configuration
    oled_write_cmd(0x12);
    oled_write_cmd(0xDB);//--set vcomh
    oled_write_cmd(0x40);//Set VCOM Deselect Level
    oled_write_cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    oled_write_cmd(0x02);//
    oled_write_cmd(0x8D);//--set Charge Pump enable/disable
    oled_write_cmd(0x14);//--set(0x10) disable
    oled_write_cmd(0xA4);// Disable Entire Display On (0xa4/0xa5)
    oled_write_cmd(0xA6);// Disable Inverse Display On (0xa6/a7) 
    oled_write_cmd(0xAF);//--turn on oled panel
    
    oled_write_cmd(0xAF); /*display ON*/ 
    
    oled_enable_refresh();
    oled_clear(0x00);
    oled_draw_rect(0, 0, 127, 64, 1);
}