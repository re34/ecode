#include "net_udp.h"

#include "lwip/udp.h"
#include "lwip/stats.h"
#include "lwip/pbuf.h"
#include "lwip/ip_addr.h"


#define UDP_PORT	1025

struct udp_pcb *udppcb;
struct ip_addr ipaddr;
struct pbuf *p_buf;

char udp_data[]="Hello wrold";

//UDP服务端
void udp_receive(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr * addr, UINT16_T port){
	struct ip_addr rmtaddr = *addr;

	if(p!=NULL){
		udp_sendto(upcb, p, &rmtaddr, port);
		pbuf_free(p);
	}
}

void udp_startservice(void){
	struct udp_pcb *upcb;
	upcb = udp_new();
	udp_bind(upcb, IP_ADDR_ANY, UDP_PORT);
	udp_recv(upcb, udp_receive, NULL);
}

//UDP 客户端
void udp_client_init(void){
	p_buf = pbuf_alloc(PBUF_RAW, sizeof(udp_data), PBUF_REF);
	p_buf->payload = (void *)udp_data;

	IP4_ADDR(&ipaddr, 192, 168, 1, 100);
	udppcb = udp_new();

	udp_bind(udppcb, IP_ADDR_ANY, 1023);
	udp_recv(udppcb, udp_receive, NULL);
	udp_connect(udppcb, &ipaddr, UDP_PORT);
	
}

void udp_client(void){
	udp_send(udppcb, p_buf);
}

