#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

/* Includes ------------------------------------------------------------------*/
#include "types.h"

#include "lwip/opt.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ppp_oe.h"

#include "lwip/udp.h"
#include "lwip/ip_addr.h"

#include "lwip/err.h"
#include "lwip/tcp.h"
#include "lwip/init.h"
#include "lwip/tcp_impl.h"
#include "lwip/dhcp.h"
#include "lwip/timers.h"




struct ethernet_dev{
	void (*low_level_init)(struct netif *netif);
	err_t (*low_level_output)(struct netif *netif, struct pbuf *p);
	struct pbuf *(*low_level_input)(struct netif *netif);
};

int ethernet_reigster(struct ethernet_dev *dev);
err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);

#endif