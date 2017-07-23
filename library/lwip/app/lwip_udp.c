#include "lwip_udp.h"

#include "lwip/udp.h"
#include "lwip/stats.h"
#include "lwip/pbuf.h"
#include "lwip/ip_addr.h"


struct udp_pcb *pxUdpPcb;
struct ip_addr xIpAddr;
struct pbuf *pxPbuf;

char acUdpData[] = "Helllo world";

void UDP_Receive(void *arg,struct udp_pcb * upcb, struct pbuf *p,struct ip_addr *addr, u16_t port){

	struct ip_addr rmtaddr = *addr;

	if(p!=NULL){
		udp_sendto(upcb, p, &rmtaddr, port);
		pbuf_free(p);
	}
}

void vUdpStartService(void){
	struct udp_pcb * upcb;
	upcb = udp_new();
	udp_bind(upcb, IP_ADDR_ANY, 1025);
	udp_recv(upcb, UDP_Receive, NULL);
}

void vUdpConnService(void){
	pxPbuf = pbuf_alloc(PBUF_RAW, sizeof(acUdpData), PBUF_RAM);
	pxPbuf->payload = (void *)acUdpData;

	IP4_ADDR(&xIpAddr, 192, 168, 1, 100);
	pxUdpPcb = udp_new();

	udp_bind(pxUdpPcb, IP_ADDR_ANY, 1025);
	udp_connect(pxUdpPcb, &xIpAddr, 1025);
	
}

void vUdpDemo(void){
	udp_send(pxUdpPcb, pxPbuf);
}
