#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

/* Includes ------------------------------------------------------------------*/
#include "types.h"

#include "lwip/opt.h"

#include "lwip/init.h"
#include "lwip/netif.h"
#include "netif/ethernet.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/ethip6.h"
#include "lwip/etharp.h"
#include "lwip/dhcp.h"
#include "netif/ppp/pppoe.h"
#include "lwip/timeouts.h"

struct ethernet_dev{
	void (*low_level_init)(struct netif *netif);
	err_t (*low_level_output)(struct netif *netif, struct pbuf *p);
	struct pbuf *(*low_level_input)(struct netif *netif);
};

int ethernet_reigster(struct ethernet_dev *dev);
err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);

#endif