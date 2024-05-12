#ifndef _UMEM_USER_H
#define _UMEM_USER_H

extern void *umem_malloc(unsigned int umem_size, unsigned int umem_pool);
extern void umem_free(void *umem_addr);

#endif
