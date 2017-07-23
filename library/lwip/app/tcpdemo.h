#ifndef __TCPDEMO_H__
#define __TCPDEMO_H__
#include "system.h"

#include "lwip/ip_addr.h"
#include "lwip/tcp.h"

void vTcpClientInit(void);

err_t xTcpServerRecv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
err_t xTcpServerAccept(void *arg, struct tcp_pcb *pcb, err_t err);
void vTcpServerInit(void);

#endif
