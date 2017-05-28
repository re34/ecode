#include "tft_lcd.h"
#include "ecode.h"

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
	tft_dev = dev;

	tft_init();
	
	return 0;
}

void _write_reg(UInt16 reg)
{
	tft_dev->rs(0);
    tft_dev->cs(0);
    tft_dev->write_bus(reg);
    tft_dev->wr(0);
    tft_dev->wr(1);
	tft_dev->write_bus(reg<<8);
    tft_dev->wr(0);
    tft_dev->wr(1);
    tft_dev->cs(1);
}

void _write_data(UInt16 data)
{
	tft_dev->rs(1);
    tft_dev->cs(0);
    tft_dev->write_bus(data);
    tft_dev->wr(0);
    tft_dev->wr(1);
	tft_dev->write_bus(data<<1);
    tft_dev->wr(0);
    tft_dev->wr(1);
    tft_dev->cs(1);
}

void tft_write_reg(UInt8 reg, UInt16 val)
{
    _write_reg(reg);
    _write_data(val);
}

void tft_prepare_ram(void)
{
    _write_reg(0x22);
}

static void tft_init(void)
{
	tft_dev->reset(1);
	delay_ms(5);
	tft_dev->reset(0);
	delay_ms(100);
	tft_dev->reset(1);
	delay_ms(50);
	
	//************* Start Initial Sequence **********//		
	tft_write_reg(0x00E5,0x78F0); 
	tft_write_reg(0x0001,0x0100); 
	tft_write_reg(0x0002,0x0700); 
	tft_write_reg(0x0003,0x1030); 
	tft_write_reg(0x0004,0x0000); 
	tft_write_reg(0x0008,0x0202);  
	tft_write_reg(0x0009,0x0000);
	tft_write_reg(0x000A,0x0000); 
	tft_write_reg(0x000C,0x0000); 
	tft_write_reg(0x000D,0x0000);
	tft_write_reg(0x000F,0x0000);
	//power on sequence VGHVGL
	tft_write_reg(0x0010,0x0000);   
	tft_write_reg(0x0011,0x0007);  
	tft_write_reg(0x0012,0x0000);  
	tft_write_reg(0x0013,0x0000); 
	tft_write_reg(0x0007,0x0000); 
	//vgh 
	tft_write_reg(0x0010,0x1690);   
	tft_write_reg(0x0011,0x0227);
	//delay_ms(100);
	//vregiout 
	tft_write_reg(0x0012,0x009D); //0x001b
	//delay_ms(100); 
	//vom amplitude
	tft_write_reg(0x0013,0x1900);
	//delay_ms(100); 
	//vom H
	tft_write_reg(0x0029,0x0025); 
	tft_write_reg(0x002B,0x000D); 
	//gamma
	tft_write_reg(0x0030,0x0007);
	tft_write_reg(0x0031,0x0303);
	tft_write_reg(0x0032,0x0003);// 0006
	tft_write_reg(0x0035,0x0206);
	tft_write_reg(0x0036,0x0008);
	tft_write_reg(0x0037,0x0406); 
	tft_write_reg(0x0038,0x0304);//0200
	tft_write_reg(0x0039,0x0007); 
	tft_write_reg(0x003C,0x0602);// 0504
	tft_write_reg(0x003D,0x0008); 
	//ram
	tft_write_reg(0x0050,0x0000); 
	tft_write_reg(0x0051,0x00EF);
	tft_write_reg(0x0052,0x0000); 
	tft_write_reg(0x0053,0x013F);  
	tft_write_reg(0x0060,0xA700); 
	tft_write_reg(0x0061,0x0001); 
	tft_write_reg(0x006A,0x0000); 
	//
	tft_write_reg(0x0080,0x0000); 
	tft_write_reg(0x0081,0x0000); 
	tft_write_reg(0x0082,0x0000); 
	tft_write_reg(0x0083,0x0000); 
	tft_write_reg(0x0084,0x0000); 
	tft_write_reg(0x0085,0x0000); 
	//
	tft_write_reg(0x0090,0x0010); 
	tft_write_reg(0x0092,0x0600); 
	
	tft_write_reg(0x0007,0x0133);
	tft_write_reg(0x00,0x0022);//	


	tft_write_reg(0x03,0x1030); 
    tft_write_reg(0x0060, 0xa700);
 
	tft_clear(0x001F);
}


void tft_set_windows(UInt16 x1, UInt16 y1, UInt16 x2, UInt16 y2)
{
    _write_reg(0x50);
	_write_data(x1);
	_write_reg(0x51);		
	_write_data(x2);
	_write_reg(0x52);	
	_write_data(y1);
	_write_reg(0x53);		
	_write_data(y2);
    
    _write_reg(0x20);
    _write_data(x1);
    _write_reg(0x21);
    _write_reg(y1);
    
    tft_prepare_ram();
}

void tft_clear(UInt16 c)
{
	UInt32 index = 0;
	tft_set_windows(0,0,TFT_WIDTH-1,TFT_HIGH-1);
    for(index=0;index<TFT_WIDTH*TFT_HIGH;index++)
    {
        _write_data(c);
    }
}






