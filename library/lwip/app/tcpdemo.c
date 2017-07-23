#include "tcpdemo.h"

#include "string.h"

const static UINT8_T tcp_data[] = "This is Lwip Tcp Client";


//TCP客户端
err_t xTcpClientConnected(void *arg, struct tcp_pcb *pcb, err_t err){
	tcp_write(pcb, tcp_data, sizeof(tcp_data), 0);
	tcp_close(pcb);

	return ERR_OK;
}

void vTcpClientInit(void){
	struct tcp_pcb *pcb;
	struct ip_addr ipaddr;

	IP4_ADDR(&ipaddr, 192, 168, 1, 100);

	pcb = tcp_new();
	tcp_bind(pcb, IP_ADDR_ANY, 1026);

	tcp_connect(pcb, &ipaddr, 1026, xTcpClientConnected);
}

//TCP服务端


UINT8_T sndbuf[50], cmdbuf[20];
INT8_T cmd_flag;


err_t xTcpServerRecv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err){

	if(p!=NULL){
		UINT16_T len;
		UINT8_T *datab;
		UINT16_T strlen;

		tcp_recved(pcb, p->tot_len);
		len = p->len;
		datab = (UINT8_T *)p->payload;
		if((len==2)&&(*datab==0x0d)&&(*(datab+1)==0x0a)){
			if(cmd_flag>0){
				cmdbuf[cmd_flag]=0x00;
				if(strcmp((char const *)cmdbuf, "hello")==0){
					strlen = sprintf((char *)sndbuf, "Hello, Nice to see you!\r\n");
					tcp_write(pcb, sndbuf, strlen, TCP_WRITE_FLAG_COPY);
				}else if(strcmp((char const *)cmdbuf,"quit")==0){
					cmd_flag = 0;
					pbuf_free(p);
					return tcp_close(pcb);
				}else{
					strlen = sprintf((char *)sndbuf, "\r\nUnknown Command: %s.\r\n", cmdbuf);
					tcp_write(pcb, sndbuf, strlen, TCP_WRITE_FLAG_COPY);
				}
				cmd_flag = 0;
			}
			strlen = sprintf((char *)sndbuf, "\r\nXShell>>");
			tcp_write(pcb, sndbuf, strlen, TCP_WRITE_FLAG_COPY);
		}else if((len==1)&&(*datab>=0x20)&&(*datab<=0x7e)&&(cmd_flag<19)){
			cmdbuf[cmd_flag]=*datab;
			cmd_flag++;
		}else if((len==1)&&(*datab==0x08)&&(cmd_flag>0)){
			cmd_flag--;
			strlen = sprintf((char *)sndbuf, "\b\b");
			tcp_write(pcb, sndbuf, strlen, TCP_WRITE_FLAG_COPY);
		}else if((len==1)&&(*datab==0x08)){
			cmd_flag=0;
			strlen = sprintf((char *)sndbuf, ">");
			tcp_write(pcb, sndbuf, strlen, TCP_WRITE_FLAG_COPY);
		}
		pbuf_free(p);
	}else{
		cmd_flag = 0;
		return tcp_close(pcb);
	}
	return ERR_OK;
}

err_t xTcpServerAccept(void *arg, struct tcp_pcb *pcb, err_t err){
	UINT16_T strlen;

	tcp_recv(pcb, xTcpServerRecv);
	strlen = sprintf((char *)sndbuf, "##Welcome to demo Telenet based on LwIP##\r\n");

	tcp_write(pcb, sndbuf, strlen, TCP_WRITE_FLAG_COPY);

	strlen = sprintf((char *)sndbuf, "##Created by Arno...                                 ##\r\n");
	tcp_write(pcb, sndbuf, strlen, TCP_WRITE_FLAG_COPY);

	strlen = sprintf((char *)sndbuf, "##quit:退出                                                 ##\r\n");
	tcp_write(pcb, sndbuf, strlen, TCP_WRITE_FLAG_COPY);

	strlen = sprintf((char *)sndbuf, "XShell>>");
	tcp_write(pcb, sndbuf, strlen, 1);

	return ERR_OK;
}


void vTcpServerInit(void){
	struct tcp_pcb *pcb;

	pcb = tcp_new();
	tcp_bind(pcb, IP_ADDR_ANY, 23);
	pcb = tcp_listen(pcb);
	tcp_accept(pcb, xTcpServerAccept);
}

