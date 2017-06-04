#include "eth.h"
#include "ecode.h"
#include "lan8742.h"
#include "etharp.h"

#if defined ( __CC_ARM )  /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */

#define TCP_TIMER_INTERVAL          250
#define ARP_TIMER_INTERVAL          5000


struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;

uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];


void ethernet_task(void *args);

void eth_init(void)
{
	lan8742_init();
	
	lwip_init();
    
#if LWIP_DHCP==1    
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
#else
    IP_ADDRESS[0] = 192;
    IP_ADDRESS[1] = 168;
    IP_ADDRESS[2] = 0;
    IP_ADDRESS[3] = 105;
    NETMASK_ADDRESS[0] = 255;
    NETMASK_ADDRESS[1] = 255;
    NETMASK_ADDRESS[2] = 255;
    NETMASK_ADDRESS[3] = 0;
    GATEWAY_ADDRESS[0] = 192;
    GATEWAY_ADDRESS[1] = 168;
    GATEWAY_ADDRESS[2] = 0;
    GATEWAY_ADDRESS[3] = 1; 
    
    /* IP addresses initialization without DHCP (IPv4) */
    IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
    IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
    IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);
#endif
	
	netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

	netif_set_default(&gnetif);
	
	if(netif_is_link_up(&gnetif))
	{
		netif_set_up(&gnetif);
	}
	else
	{
		netif_set_down(&gnetif);
	}
    
#if LWIP_DHCP==1
	dhcp_start(&gnetif);
#endif

	xTaskCreate(ethernet_task,
            "ethernet_task",
            1024,
            NULL,
            1,
            NULL);
}




void ethernet_process(void)
{
    ethernetif_input(&gnetif);
    sys_check_timeouts();

}

void ethernet_task(void *args)
{
    unsigned int tcp_timer = 0;
    unsigned int arp_timer = 0;
    unsigned int jiffies = 0;
    
    jiffies = get_ticks();
    tcp_timer = jiffies;
    arp_timer = jiffies;
    while(1)
    {
        ethernet_process();
        /*
        jiffies = get_ticks();
        if(time_after(jiffies, tcp_timer+TCP_TIMER_INTERVAL))
        {
            tcp_timer = jiffies;
            tcp_tmr();
        }
        if(time_after(jiffies, arp_timer+ARP_TIMER_INTERVAL))
        {
            arp_timer = jiffies;
            etharp_tmr();
        }
        */
    }
}


#if defined ( __CC_ARM )  /* MDK ARM Compiler */
/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
  sio_fd_t sd;

/* USER CODE BEGIN 7 */
  sd = 0; // dummy code
/* USER CODE END 7 */
	
  return sd;
}

/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{
/* USER CODE BEGIN 8 */
/* USER CODE END 8 */
}

/**
 * Reads from the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 *
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 9 */
  recved_bytes = 0; // dummy code
/* USER CODE END 9 */	
  return recved_bytes;
}

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 10 */
  recved_bytes = 0; // dummy code
/* USER CODE END 10 */	
  return recved_bytes;
}
#endif /* MDK ARM Compiler */
