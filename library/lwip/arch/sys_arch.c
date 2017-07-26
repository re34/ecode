#include "cc.h"
#include "types.h"
#include "ecode.h"
u32_t sys_now(void)
{
	return system_get_time();
}

u32_t sys_jiffies(void)
{
    return system_get_time();
}
/*
void sys_init(void)
{

}

err_t sys_sem_new(sys_sem_t * sem, u8_t count)
{

	return ERR_OK;
}

void sys_sem_free(sys_sem_t * sem)
{

}

u32_t sys_arch_sem_wait(sys_sem_t * sem, u32_t timeout)
{
	
	return 0;
}

err_t sys_mbox_new(sys_mbox_t * mbox, int size)
{
	
	return ERR_OK;
}

void sys_mbox_free(sys_mbox_t * mbox)
{
	
}

void sys_mbox_post(sys_mbox_t * mbox, void *msg)
{
	
}

err_t sys_mbox_trypost(sys_mbox_t * mbox, void * msg)
{
	return ERR_OK;
}

u32_t sys_arch_mbox_fetch(sys_mbox_t * mbox, void ** msg, u32_t timeout)
{
	return 0;
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t * mbox, void ** msg)
{
	return 0;
}

int sys_mbox_valid(sys_mbox_t * mbox)
{
	return 1;
}
void sys_mbox_set_invalid(sys_mbox_t * mbox)
{
	
}

sys_thread_t sys_thread_new(char * name, void (* thread)(void * arg), void * arg, int stacksize, int prio)
{

}

sys_prot_t sys_arch_protect(void)
{

}

void sys_arch_unprotect(sys_prot_t pval)
{

}

*/
