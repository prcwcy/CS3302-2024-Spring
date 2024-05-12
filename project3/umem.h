#ifndef _UMEM_H
#define _UMEM_H

#include <linux/types.h>
#include <linux/ioctl.h>

#define UMEM_NUM_POOL 4
#define UMEM_POOL_SIZE 4

#define UMEM_NAME "umem"
#define UMEM_PATH_NAME "/dev/" UMEM_NAME

#define UMEM_IOC_MAGIC '0'

struct umem_info
{
    __u64 umem_size;
    __u64 umem_pool;
    __u64 umem_addr;
};

#define UMEM_IOC_MALLOC _IOW(UMEM_IOC_MAGIC, 0, struct umem_info)
#define UMEM_IOC_FREE _IOW(UMEM_IOC_MAGIC, 1, struct umem_info)
#define UMEM_IOC_PAGE_FAULT _IOW(UMEM_IOC_MAGIC, 2, struct umem_info)

#endif
