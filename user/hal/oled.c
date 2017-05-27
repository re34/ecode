#include "oled.h"
#include "oledfont.h"


struct oled_dev *oled_dev;

static void _oled_init();

int oled_register(struct oled_dev *dev)
{
	if(oled_dev==NULL)
		return -1;
	if(dev->init()<0)
		return -2;
	oled_dev = dev;
	_oled_init();
	
	return 0;
}

void oled_write_byte(unsigned char data, unsigned char cmd)
{
	unsigned char i;
	
	if(cmd)
		oled_dev->dc(BIT_HIGH);
	else
		oled_dev->dc(BIT_LOW);
	oled_dev->cs(BIT_LOW);
	for(i=0;i<8;i++)
	{
		oled_dev->sclk(BIT_LOW);
		if(data&0x80)
			oled_dev->sdin(BIT_HIGH);
		else
			oled_dev->sdin(BIT_LOW);
		oled_dev->sclk(BIT_HIGH);
		data<<=1;
	}
	oled_dev->cs(BIT_HIGH);
	oled_dev->dc(BIT_HIGH);
}

void oled_set_pos(unsigned char x, unsigned char y)
{
	oled_write_byte(0xb0+y,OLED_CMD);
	oled_write_byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	oled_write_byte((x&0x0f)|0x01,OLED_CMD); 
}

void oled_display_on()
{
	oled_write_byte(0X8D,OLED_CMD);  //SET DCDC命令
	oled_write_byte(0X14,OLED_CMD);  //DCDC ON
	oled_write_byte(0XAF,OLED_CMD);  //DISPLAY ON
}

void oled_display_off()
{
	oled_write_byte(0X8D,OLED_CMD);  //SET DCDC命令
	oled_write_byte(0X10,OLED_CMD);  //DCDC OFF
	oled_write_byte(0XAE,OLED_CMD);  //DISPLAY OFF
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void oled_clear(void)  
{  
	UInt8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		oled_write_byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		oled_write_byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		oled_write_byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)oled_write_byte(0,OLED_DATA); 
	} //更新显示
}

void oled_show_char(UInt8 x, UInt8 y, UInt8 chr)
{
	UInt8 c=0;
	UInt8 i=0;

	c = chr-' ';
	if(x>Max_Column-1){x=0;y=y+2;}
	if(SIZE ==16)
	{
		oled_set_pos(x,y);	
		for(i=0;i<8;i++)
			oled_write_byte(F8X16[c*16+i],OLED_DATA);
		oled_set_pos(x,y+1);
		for(i=0;i<8;i++)
			oled_write_byte(F8X16[c*16+i+8],OLED_DATA);
	}
	else 
	{	
		oled_set_pos(x,y+1);
		for(i=0;i<6;i++)
			oled_write_byte(F6x8[c][i],OLED_DATA);
				
	}
	
}

//m^n函数
UInt32 oled_pow(UInt8 m,UInt8 n)
{
	UInt32 result=1;	 
	while(n--)result*=m;    
	return result;
}

void oled_show_num(UInt8 x, UInt8 y, UInt32 num, UInt8 len, UInt8 size)
{
	UInt8 t,temp;
	UInt8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
}

void oled_show_string(UInt8 x, UInt8 y, UInt8 *str)
{
	unsigned char j=0;
	while (str[j]!='\0')
	{		OLED_ShowChar(x,y,str[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

void oled_show_chinese(UInt8 x, UInt8 y, UInt8 no)
{
	
}

void oled_draw_bmp(UInt8 x0, UInt8 y0, UInt8 x1, UInt8 y1, UInt8 *bmp)
{
	unsigned int j=0;
	unsigned char x,y;
  
	if(y1%8==0) y=y1/8;      
	else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		oled_set_pos(x0,y);
		for(x=x0;x<x1;x++)
	    {      
	    	oled_write_byte(bmp[j++],OLED_DATA);	    	
	    }
	}
}

static void _oled_init()
{
	oled_dev->reset(BIT_HIGH);
	delay_ms(100);
	oled_dev->reset(BIT_LOW);
	delay_ms(200);
	oled_dev->reset(BIT_HIGH);
	
	oled_write_byte(0xAE,OLED_CMD);//--turn off oled panel
	oled_write_byte(0x00,OLED_CMD);//---set low column address
	oled_write_byte(0x10,OLED_CMD);//---set high column address
	oled_write_byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	oled_write_byte(0x81,OLED_CMD);//--set contrast control register
	oled_write_byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	oled_write_byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	oled_write_byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	oled_write_byte(0xA6,OLED_CMD);//--set normal display
	oled_write_byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	oled_write_byte(0x3f,OLED_CMD);//--1/64 duty
	oled_write_byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	oled_write_byte(0x00,OLED_CMD);//-not offset
	oled_write_byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	oled_write_byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	oled_write_byte(0xD9,OLED_CMD);//--set pre-charge period
	oled_write_byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	oled_write_byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	oled_write_byte(0x12,OLED_CMD);
	oled_write_byte(0xDB,OLED_CMD);//--set vcomh
	oled_write_byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	oled_write_byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	oled_write_byte(0x02,OLED_CMD);//
	oled_write_byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	oled_write_byte(0x14,OLED_CMD);//--set(0x10) disable
	oled_write_byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	oled_write_byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	oled_write_byte(0xAF,OLED_CMD);//--turn on oled panel
	
	oled_write_byte(0xAF,OLED_CMD); /*display ON*/ 
	oled_clear();
	oled_set_pos(0,0); 		
}

