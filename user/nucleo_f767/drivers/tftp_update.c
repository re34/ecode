#include "tftp_update.h"
#include "ecode.h"
#include <string.h>

#define FLASH_UPDATE_ADDRESS        ((e_uint32_t)0x08180000)
#define FLASH_UPDATE_SIZE           (256*1024*2)
#define FLASH_UPDATE_SECTOR_1       FLASH_SECTOR_10
#define FLASH_UPDATE_SECTOR_2       FLASH_SECTOR_11


static void * tftp_open(const char *fname, const char *mode, u8_t write);
static void tftp_close(void *handle);
static int tftp_read(void *handle, void *buf, int bytes);
static int tftp_write(void *handle, struct pbuf *p);


static const struct tftp_context ftftp_server={
    .open = tftp_open,
    .close = tftp_close,
    .read = tftp_read,
    .write = tftp_write,
};

struct tftp_file{
    e_uint32_t address;
    e_uint32_t offset;
};

static struct tftp_file ftftp_file;

e_err_t tftp_update_init(void)
{
    if(tftp_init(&ftftp_server)!=ERR_OK)
        return -E_ERROR;
    
    return E_EOK;
}

static void * tftp_open(const char *fname, const char *mode, u8_t write)
{
  
    if(strncmp(fname,"ecode",5)!=0)
    {
        return NULL;
    }
    HAL_FLASH_Unlock();
    ftftp_file.address = FLASH_UPDATE_ADDRESS;
    ftftp_file.offset = 0;
    
    HAL_StatusTypeDef ret;
    uint32_t sector_err;
    FLASH_EraseInitTypeDef FlashEraseInitStruct;
    
    FlashEraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    FlashEraseInitStruct.Sector = FLASH_UPDATE_SECTOR_1;
    FlashEraseInitStruct.NbSectors = 1;
    FlashEraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    
    ret = HAL_FLASHEx_Erase(&FlashEraseInitStruct, &sector_err);
    
    if(ret!=HAL_OK)
        return NULL;
    
    FlashEraseInitStruct.Sector = FLASH_UPDATE_SECTOR_2;
    
    ret = HAL_FLASHEx_Erase(&FlashEraseInitStruct, &sector_err);
    
    if(ret!=HAL_OK)
        return NULL;
  
    return &ftftp_file;
}

static void tftp_close(void *handle)
{
    HAL_FLASH_Lock();
}

static int tftp_read(void *handle, void *buf, int bytes)
{
    
    return 0;
}

static int tftp_write(void *handle, struct pbuf *p)
{
    struct tftp_file *file = handle;
    e_uint32_t address;
    e_uint32_t word_count;
    e_uint32_t buffer[128];
    e_uint16_t i;

    
    if(handle==NULL)
        return 0;
    address = file->address+file->offset;
    LOG_DEBUG("write %d bytes to 0x%x", p->len, address);

    pbuf_copy_partial(p, buffer, p->len, 0);
    word_count = p->len/4;
    if((p->len%4)!=0)
      word_count++;
    for(i=0;i<word_count;i++)
    {
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, buffer[i])!=HAL_OK)
            return -1;
        address += 4;
    }
    file->offset+=p->len;
    

    return 0;
}







