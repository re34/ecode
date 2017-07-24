#include "eth.h"
#include "ecode.h"
#include "lan8742.h"


#if defined ( __CC_ARM )  /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */

#define TCP_TIMER_INTERVAL          250
#define ARP_TIMER_INTERVAL          5000

#define IP_ADDR0    192
#define IP_ADDR1    168
#define IP_ADDR2    0
#define IP_ADDR3    119

#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

#define GW_ADDR0        192
#define GW_ADDR1        168
#define GW_ADDR2        0
#define GW_ADDR3        1


#define MAX_DHCP_TRIES      10

enum{
    DHCP_OFF,
    DHCP_START,
    DHCP_WAIT_ADDRESS,
    DHCP_ADDRESS_ASSIGNED,
    DHCP_TIMEOUT,
    DHCP_LINK_DOWN,
};

struct netif gnetif;
ip_addr_t ipaddr;
ip_addr_t netmask;
ip_addr_t gw;

uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];
uint8_t dhcp_state = 0;


static SemaphoreHandle_t xSemaphoreEth;
void ethernet_rx_indicate(void);

void ethernet_task(void *args);
void ethernet_data_task(void *args);

void eth_init(void)
{
    struct lan8742_operations lan8742_ops;
    lan8742_ops.indicate = ethernet_rx_indicate;
    xSemaphoreEth = xSemaphoreCreateBinary();
    
	lan8742_init(lan8742_ops);
	
	lwip_init();
    
#if LWIP_DHCP==1    
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
#else
    IP_ADDRESS[0] = IP_ADDR0;
    IP_ADDRESS[1] = IP_ADDR1;
    IP_ADDRESS[2] = IP_ADDR2;
    IP_ADDRESS[3] = IP_ADDR3;
    NETMASK_ADDRESS[0] = NETMASK_ADDR0;
    NETMASK_ADDRESS[1] = NETMASK_ADDR1;
    NETMASK_ADDRESS[2] = NETMASK_ADDR2;
    NETMASK_ADDRESS[3] = NETMASK_ADDR3;
    GATEWAY_ADDRESS[0] = GW_ADDR0;
    GATEWAY_ADDRESS[1] = GW_ADDR1;
    GATEWAY_ADDRESS[2] = GW_ADDR2;
    GATEWAY_ADDRESS[3] = GW_ADDR3; 
    
    /* IP addresses initialization without DHCP (IPv4) */
    IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
    IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
    IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);
    LOG_DEBUG("ip address: %d.%d.%d.%d",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
    LOG_DEBUG("net mask: %d.%d.%d.%d", NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
    LOG_DEBUG("gateway: %d.%d.%d.%d", GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
#endif
	
	netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

	netif_set_default(&gnetif);
	
	if(netif_is_link_up(&gnetif))
	{
		netif_set_up(&gnetif);
        LOG_DEBUG("netif link up!");
	}
	else
	{
		netif_set_down(&gnetif);
	}
    
#if LWIP_DHCP==1
    if(netif_is_up(&gnetif))
    {
        dhcp_state = DHCP_START;
    }
#endif
    
	xTaskCreate(ethernet_task,
            "ethernet_task",
            512,
            NULL,
            1,
            NULL);
    
	xTaskCreate(ethernet_data_task,
            "ethernet_data_task",
            512,
            NULL,
            5,
            NULL);
}

void dhcp_process(void)
{
    struct dhcp *dhcp;
    static e_uint32_t dhcp_timer = 0;
    e_uint32_t dhcp_local_timer = 0;
    if(dhcp_timer==0)
        dhcp_timer = system_get_time();
    dhcp_local_timer = system_get_time();
    if(dhcp_local_timer-dhcp_timer>250)
    {
        dhcp_local_timer = dhcp_timer;
        switch(dhcp_state)
        {
        case DHCP_START:
            ip_addr_set_zero_ip4(&gnetif.ip_addr);
            ip_addr_set_zero_ip4(&gnetif.netmask);
            ip_addr_set_zero_ip4(&gnetif.gw);       
            dhcp_start(&gnetif);
            dhcp_state = DHCP_WAIT_ADDRESS;
            break;
        case DHCP_WAIT_ADDRESS:
            if(dhcp_supplied_address(&gnetif))
            {
                dhcp_state = DHCP_ADDRESS_ASSIGNED;
                LOG_DEBUG("dhcp completed!", );
                LOG_DEBUG("ip address: %d.%d.%d.%d",ip4_addr1(&gnetif.ip_addr),ip4_addr2(&gnetif.ip_addr),ip4_addr3(&gnetif.ip_addr),ip4_addr4(&gnetif.ip_addr));
                LOG_DEBUG("net mask: %d.%d.%d.%d", ip4_addr1(&gnetif.netmask),ip4_addr2(&gnetif.netmask),ip4_addr3(&gnetif.netmask),ip4_addr4(&gnetif.netmask));
                LOG_DEBUG("gateway: %d.%d.%d.%d", ip4_addr1(&gnetif.gw),ip4_addr2(&gnetif.gw),ip4_addr3(&gnetif.gw),ip4_addr4(&gnetif.gw));
            }
            else
            {
                dhcp = (struct dhcp *)netif_get_client_data(&gnetif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
                if(dhcp->tries > MAX_DHCP_TRIES)
                {
                    dhcp_state = DHCP_TIMEOUT;
                    dhcp_stop(&gnetif);
                    
                    /* Static address used */
                    IP_ADDR4(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
                    IP_ADDR4(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
                    IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
                    netif_set_addr(&gnetif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));
                    LOG_DEBUG("dhcp timeout,static address was used!", );
                    LOG_DEBUG("ip address: %d.%d.%d.%d",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
                    LOG_DEBUG("net mask: %d.%d.%d.%d", NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
                    LOG_DEBUG("gateway: %d.%d.%d.%d", GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
                }
            }
            break;
        case DHCP_LINK_DOWN:
            dhcp_stop(&gnetif);
            dhcp_state = DHCP_OFF;
            break;
        }
    }
    
}

void ethernet_rx_indicate(void)
{
    xSemaphoreGive(xSemaphoreEth);
}

void ethernet_process(void)
{
    if(lan8742_get_packet_size()>0)
        ethernetif_input(&gnetif);
    //sys_check_timeouts();
}

void ethernet_task(void *args)
{
   // unsigned int tcp_timer = 0;
    //unsigned int arp_timer = 0;
    //unsigned int jiffies = 0;
    
    //jiffies = get_ticks();
    //tcp_timer = jiffies;
    //arp_timer = jiffies;
    
    
    
    while(1)
    {
        //ethernet_process();
        //delay_ms(2);
        
        //jiffies = get_ticks();
        //if(time_after(jiffies, tcp_timer+TCP_TIMER_INTERVAL))
        //{
        //    tcp_timer = jiffies;
        //    tcp_tmr();
        //}
        //if(time_after(jiffies, arp_timer+ARP_TIMER_INTERVAL))
        //{
         //   arp_timer = jiffies;
         //   etharp_tmr();
        //}
        sys_check_timeouts();
        dhcp_process();
        delay_ms(10);
        
    }
}

void ethernet_data_task(void *args)
{

  for(;;)
  {
    //xSemaphoreTake(xSemaphoreEth, portMAX_DELAY);
    ethernet_process();
    delay_ms(1);
  
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
