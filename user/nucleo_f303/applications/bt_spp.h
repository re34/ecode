#ifndef __BT_SPP_H__
#define __BT_SPP_H__

typedef enum{
    BT_COM_MODE,
    BT_CLI_MODE,
}bt_mode_t;


void bt_spp_init(void);
void bt_spp_set_mode(bt_mode_t mode);
int bt_spp_write(unsigned char *buf, int len);
#endif