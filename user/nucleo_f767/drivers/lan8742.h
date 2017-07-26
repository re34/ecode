#ifndef __LAN8742_H__
#define __LAN8742_H__

struct lan8742_operations{
    void (*indicate)(int *xhigher_priority_woken);
};

int lan8742_init(struct lan8742_operations ops);
int lan8742_get_packet_size(void);
#endif