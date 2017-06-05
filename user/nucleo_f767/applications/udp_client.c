#include "udp_client.h"

char udp_data[]="Hello wrold";

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