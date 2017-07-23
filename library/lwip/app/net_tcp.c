#include "net_tcp.h"
#include "lwip\err.h"
#include "lwip\tcp.h"
#include "lwip\pbuf.h"

#define TCP_PORT	1026
const static UINT8_T tcp_data[]="This is lwip client";
const static UINT8_T index_data[]="<html>\
								<head><title>STM32 html</title></head>\
								<body>\
								This is a LWIP web demo!\
								</body>\
								</html>";
const static UINT8_T http_hdr[]="HTTP/1.1 200 OK\r\nContent-type:text/html\r\n\r\n";

//TCP客户端
err_t tcpclient_connected(void *arg, struct tcp_pcb *pcb, err_t err){
	tcp_write(pcb, tcp_data, sizeof(tcp_data), 0);
	tcp_close(pcb);

	return ERR_OK;
}

void tcpclient_init(void){
	struct tcp_pcb *pcb;
	struct ip_addr ipaddr;

	IP4_ADDR(&ipaddr, 192, 168, 1, 100);
	pcb = tcp_new();
	tcp_bind(pcb, IP_ADDR_ANY, TCP_PORT);
	tcp_connect(pcb, &ipaddr, TCP_PORT, tcpclient_connected);
}

////////////////////////////////////////
///TCP服务端
static err_t tcpserver_recv(void *arg, struct tcp_pcb * pcb, struct pbuf *p, err_t err){
	if(p!=NULL){
		tcp_recved(pcb, p->tot_len);
		tcp_write(pcb, p->payload, p->len, 0);
		//tcp_write(pcb, http_hdr, sizeof(http_hdr), 0);
		//tcp_write(pcb, index_data, sizeof(index_data), 0);
		pbuf_free(p);
	}
	else if(err==ERR_OK){
		return tcp_close(pcb);
	}
	return ERR_OK;
}

static err_t tcpserver_accept(void *arg, struct tcp_pcb * pcb, err_t err){
	tcp_setprio(pcb, TCP_PRIO_MIN);
	
	tcp_recv(pcb, tcpserver_recv);
	return ERR_OK;
}

void tcpserver_init(void){
	struct tcp_pcb *pcb;
	pcb = tcp_new();
	tcp_bind(pcb, IP_ADDR_ANY, 8080);
	pcb = tcp_listen(pcb);
	tcp_accept(pcb, tcpserver_accept);
}

