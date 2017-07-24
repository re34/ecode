#include "tcp_client.h"
#include "ecode.h"

const static char tcp_client_data[]="This is LWIP Tcp client!";

err_t tcp_client_connedted(void *arg, struct tcp_pcb *pcb, err_t err)
{
	tcp_write(pcb, tcp_client_data, sizeof(tcp_client_data), 0);
	tcp_close(pcb);
	
	return ERR_OK;
}

void tcp_client_init(void)
{
	struct tcp_pcb *pcb;
	ip_addr_t ipaddr;
	
	IP4_ADDR(&ipaddr, 192,168,0,105);
	
	pcb = tcp_new();
	tcp_bind(pcb, IP_ADDR_ANY, 1026);
	
	tcp_connect(pcb, &ipaddr, 1026, tcp_client_connedted);
	
}