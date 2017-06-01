#ifndef LWIP_HDR_LWIPOPTS_H
#define LWIP_HDR_LWIPOPTS_H

/* Prevent having to link sys_arch.c (we don't test the API layers in unit tests) */
#define NO_SYS                          1
#define SYS_LIGHTWEIGHT_PROT            0
#define LWIP_NETCONN                    0
#define LWIP_SOCKET                     0

#define MEM_ALIGNMENT					4
#define MEM_SIZE						(10*1024)

#define MEMP_NUM_PBUF		10

#define MEMP_NUM_UDP_PCB	6
#define MEMP_NUM_TCP_PCB	10
#define MEMP_NUM_TCP_PCB_LISTEN	10
#define MEMP_NUM_TCP_SEG	20

#define MEMP_NUM_SYS_TIMEOUT	5
#define PBUF_POOL_SIZE		10
#define PBUF_POOL_BUFSIZE	1500

#define LWIP_TCP	1
#define TCP_TTL		255

#define TCP_QUEUE_OOSEQ	0	
#define TCP_SND_BUF		(4*TCP_MSS)
#define TCP_SND_QUEUELEN	(4*TCP_SND_BUF/TCP_MSS)
#define TCP_WND		(2*TCP_MSS)
#define LWIP_ICMP	1

#define LWIP_UDP	1
#define UDP_TTL		255

#define LWIP_DHCP	0
#define DHCP_DOES_ARP_CHECK 0

#define LWIP_STATS	0
#define LWIP_PROVIDE_ERRNO		1

#endif /* LWIP_HDR_LWIPOPTS_H */