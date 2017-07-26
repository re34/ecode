#include "update.h"
#include "ecode.h"

#define FLASH_UPDATE_ADDRESS        ((e_uint32_t)0x08180000)
#define FLASH_UPDATE_SIZE           (256*1024*2)
#define FLASH_UPDATE_SECTOR_1       FLASH_SECTOR_10
#define FLASH_UPDATE_SECTOR_2       FLASH_SECTOR_11


e_err_t update_erea_erase(void)
{
    HAL_StatusTypeDef ret;
    uint32_t sector_err;
    FLASH_EraseInitTypeDef FlashEraseInitStruct;
    
    FlashEraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    FlashEraseInitStruct.Sector = FLASH_UPDATE_SECTOR_1;
    FlashEraseInitStruct.NbSectors = 1;
    FlashEraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    
    ret = HAL_FLASHEx_Erase(&FlashEraseInitStruct, &sector_err);
    
    if(ret!=HAL_OK)
        return -E_ERROR;
    
    FlashEraseInitStruct.Sector = FLASH_UPDATE_SECTOR_2;
    
    ret = HAL_FLASHEx_Erase(&FlashEraseInitStruct, &sector_err);
    
    if(ret!=HAL_OK)
        return -E_ERROR;
    
    return E_EOK;
}

int update_data_write(e_uint32_t address, void *buffer, e_size_t size)
{
    uint32_t data;
    uint32_t word_count = size/4;
    uint8_t rem_count = size%4;
    int8_t i;
    uint8_t * pbuffer = buffer;
    
    
    while(word_count>0)
    {
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, *((uint32_t *)pbuffer))!=HAL_OK)
            return -E_ERROR;
        address += 4;
        pbuffer += 4;
        word_count--;
    }
    if(rem_count>0)
    {
        data = 0;
        for(i=rem_count-1;i>=0;i--)
        {
            data<<=8;
            data |= ((uint8_t *)pbuffer)[i];
        }
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data)!=HAL_OK)
            return -E_ERROR;
    }
    
    return 0;
}

e_err_t update_process(struct firmware *firmware)
{
    e_err_t ret;
    e_uint32_t bytes_received;
    HAL_FLASH_Unlock();
    
    print_log("Flash erase...\r\n");
    
    ret = firmware_erase(firmware);
    
    if(ret!=E_EOK)
    {
        goto update_error;
    }
    
    print_log("Firmware upgrading...\r\n");
    
    taskENTER_CRITICAL();
    ret = XmodemReceive(FLASH_UPDATE_SIZE, &bytes_received, FLASH_UPDATE_ADDRESS, update_data_write);
    taskEXIT_CRITICAL();
    
    if(ret!=E_EOK)
    {
        firmware_erase(firmware);
        goto update_error;
    }
    
    print_log("Firmware upgrade finished!\r\n");
    
    return E_EOK;
    
update_error:
    HAL_FLASH_Lock();
    
    return ret;
}

static const struct firmware_operations firmware_ops={
    .erase = update_erea_erase,
    .write = update_data_write,
    .update = update_process,
};

static struct firmware _firmware={
    .ops = &firmware_ops,
};


static int xmodem_getc(void)
{
    int ch = -1;
    
    serial_read(COM1, &ch, 1);
    
    return ch;
}

static void xmodem_putc(e_uint8_t ch)
{
    serial_write(COM1, &ch, 1);
}

void update_init(void)
{
    XmodemDescription_t fxmodem_desc;
    fxmodem_desc.fputc = xmodem_putc;
    fxmodem_desc.fgetc = xmodem_getc;
    
    XmodemInit(fxmodem_desc);
    
    firmware_register(&_firmware);
}