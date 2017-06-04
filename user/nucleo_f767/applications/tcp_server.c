#include "tcp_server.h"
#include "ecode.h"
#include "lwip/tcp.h"
#include <string.h>


UInt8 sndbuf[50], cmdbuf[20];
Int8 cmd_flag;

static err_t tcp_server_accept(void *arg, struct tcp_pcb *pcb, err_t err);

void tcp_server_init(void)
{
    struct tcp_pcb *pcb;
    
    pcb = tcp_new();
    tcp_bind(pcb, IP_ADDR_ANY, 6060);
    
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, tcp_server_accept);
}


err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    if(p!=NULL)
    {
        tcp_recved(pcb, p->tot_len);
        tcp_write(pcb, p->payload, p->len, 1);
        pbuf_free(p);
    }else if(err==ERR_OK){
        return tcp_close(pcb);
    }
    return ERR_OK;
}

static err_t tcp_server_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
    tcp_recv(pcb, tcp_server_recv);
    return ERR_OK;
}



