#include "ethernetif.h"
#include "ecode.h"


#define IFNAME0 'e'
#define IFNAME1 'c'


struct ethernet_dev *ethernet_dev;

int ethernet_reigster(struct ethernet_dev *dev)
{
	if(dev==NULL)
		return -1;
	
	ethernet_dev = dev;
	
	return 0;
}


/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void ethernetif_input(struct netif *netif)
{
  struct pbuf *p=NULL;

  do{
      p = ethernet_dev->low_level_input(netif);
      if(p!=NULL)
      {
          if(netif->input(p, netif)!=ERR_OK)
          {
              pbuf_free(p);
          }
      }
  }while(p!=NULL);
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));
  
#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */

  //netif->linkoutput = low_level_output;
  netif->output =  etharp_output;
  netif->linkoutput = ethernet_dev->low_level_output;

  /* initialize the hardware */
  //low_level_init(netif);
  ethernet_dev->low_level_init(netif);
  
  return ERR_OK;
}





/* USER CODE END 9 */