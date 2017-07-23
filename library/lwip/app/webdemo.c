#include "webdemo.h"
#include "lwip/tcp.h"
#include "lwip/pbuf.h"

const static UINT8_T indexdata[]="<html>\
<head><title>STM32 web demo</title></head>\
<body>\
Lwip TCB on Cortex-M4 testing!\
</body>\
</html>";

const static UINT8_T http_html_hdr[]="HTTP/1.1 200 OK\r\nContent-type:text/html\r\n\r\n";

static err_t xHttpRecv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err){
	if(p!=NULL){
		tcp_write(pcb, http_html_hdr, sizeof(http_html_hdr), 0);
		tcp_write(pcb, indexdata, sizeof(indexdata), 0);

		pbuf_free(p);
	}

	tcp_close(pcb);

	err = ERR_OK;

	return err;
}

static err_t xHttpAccept(void *arg, struct tcp_pcb *pcb, err_t err){

	tcp_setprio(pcb, TCP_PRIO_MIN);

	tcp_recv(pcb, xHttpRecv);

	err = ERR_OK;

	return err;
}

void vHttpInit(void){
	struct tcp_pcb *pcb;

	pcb = tcp_new();
	tcp_bind(pcb, IP_ADDR_ANY, 80);
	pcb = tcp_listen(pcb);

	tcp_accept(pcb, xHttpAccept);
}