#include "bootloader.h"
#include "ecode.h"


//Firmware 升级和运行区域划分
//升级区域
#define FLASH_FROM_SECTOR_ADDRESS             ((uint32_t)0x08180000)
#define FLASH_FROM_SECTOR_SIZE                (256*1024*2)
#define FLASH_FROM_SECTOR_1                   FLASH_SECTOR_10
#define FLASH_FROM_SECTOR_2                   FLASH_SECTOR_11


//运行区域
#define FLASH_TO_SECTOR_ADDRESS               ((e_uint32_t)0x08040000)
#define FLASH_TO_SECTOR_SIZE                  (256*1024*2)
#define FLASH_TO_SECTOR_1                      FLASH_SECTOR_5
#define FLASH_TO_SECTOR_2                      FLASH_SECTOR_6


void firmware_device_open(void)
{
    HAL_FLASH_Unlock();
}

void firmware_device_close(void)
{
    HAL_FLASH_Lock();
}

e_err_t firmware_area_check(e_uint32_t address)
{
    if(((*(__IO e_uint32_t*)address)&0x2FFE0000)==0x20000000){
        return E_EOK;
    }
    return -E_ERROR;
}

e_err_t firmware_running_erea_erase(void)
{
    HAL_StatusTypeDef ret;
    uint32_t sector_err;
    FLASH_EraseInitTypeDef FlashEraseInitStruct;
    
    FlashEraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    FlashEraseInitStruct.Sector = FLASH_TO_SECTOR_1;
    FlashEraseInitStruct.NbSectors = 1;
    FlashEraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    
    ret = HAL_FLASHEx_Erase(&FlashEraseInitStruct, &sector_err);
    
    if(ret!=HAL_OK)
        return -E_ERROR;
    
    FlashEraseInitStruct.Sector = FLASH_TO_SECTOR_2;
    
    ret = HAL_FLASHEx_Erase(&FlashEraseInitStruct, &sector_err);
    
    if(ret!=HAL_OK)
        return -E_ERROR;
    
    return E_EOK;
}

e_err_t firmware_update_area_erase(void)
{
    HAL_StatusTypeDef ret;
    uint32_t sector_err;
    FLASH_EraseInitTypeDef FlashEraseInitStruct;
    
    FlashEraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    FlashEraseInitStruct.Sector = FLASH_FROM_SECTOR_1;
    FlashEraseInitStruct.NbSectors = 1;
    FlashEraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    
    ret = HAL_FLASHEx_Erase(&FlashEraseInitStruct, &sector_err);
    
    if(ret!=HAL_OK)
        return -E_ERROR;
    
    FlashEraseInitStruct.Sector = FLASH_FROM_SECTOR_1;
    
    ret = HAL_FLASHEx_Erase(&FlashEraseInitStruct, &sector_err);
    
    if(ret!=HAL_OK)
        return -E_ERROR;
    
    return E_EOK;
}

e_err_t firmware_data_write(e_uint32_t address, void *buffer, e_size_t size)
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
    
    return E_EOK;
}

e_err_t firmware_update_application(void)
{
    e_int32_t i;
    e_uint32_t dest_address;
    e_uint32_t src_address;
    
    dest_address = FLASH_TO_SECTOR_ADDRESS;
    src_address = FLASH_FROM_SECTOR_ADDRESS;
    
    for(i=0;i<FLASH_TO_SECTOR_SIZE/4;i++)
    {
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, dest_address, *((uint32_t *)src_address))!=HAL_OK)
            return -E_ERROR;
        dest_address+=4;
        src_address += 4;
    }
    return E_EOK;
}

typedef void (*application_t)(void);
void firmware_running_application(void)
{
    application_t fapplication;
    e_uint32_t application_address;
    
    application_address = *(__IO e_uint32_t *)(FLASH_TO_SECTOR_ADDRESS+4);
    fapplication = (application_t)application_address;
    LOG_DEBUG("application address=0x%x, MSP=0x%x", application_address, FLASH_TO_SECTOR_ADDRESS);
    __set_MSP(*(__IO e_uint32_t*)FLASH_TO_SECTOR_ADDRESS);
    fapplication();
}

e_err_t bootloader_running(void)
{
    e_err_t err=E_EOK;
    firmware_device_open();
    
    if(firmware_area_check(FLASH_FROM_SECTOR_ADDRESS)!=E_EOK)
    {
        //LOG_DEBUG("no firmware found!application will running normal!");
        err = -E_ERROR;
        goto bootloader_application;
    }
    LOG_DEBUG("New firmware found!");
    LOG_DEBUG("application area erase...");
    if(firmware_running_erea_erase()!=E_EOK)
    {
        LOG_DEBUG("firmware running erea erase failed!");
        err = -E_ERROR;
        goto bootloader_error;
    }
    LOG_DEBUG("firmware updating...");
    if(firmware_update_application()!=E_EOK)
    {
        LOG_DEBUG("application firmware update failed!");
        err = -E_ERROR;
        goto bootloader_error;
    }
    LOG_DEBUG("firmware area erasing...");
    if(firmware_update_area_erase()!=E_EOK)
    {
        LOG_DEBUG("firmware area erase failed!");
        err = -E_ERROR;
        goto bootloader_error;
    }
    LOG_DEBUG("application will running...");

bootloader_application:
    firmware_device_close();
    firmware_running_application();
    return E_EOK;
    
bootloader_error:
    firmware_device_close();
    return err;
}





