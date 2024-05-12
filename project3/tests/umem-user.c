#include "../umem.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

static int umem_fd = -1;
static struct sigaction oldact;

static void umem_sa_sigaction(int sig, siginfo_t *info, void *ucontext)
{
    struct umem_info user_umem_info = {
        .umem_addr = (__u64)info->si_addr,
    };
    // printf("SIGSEGV received: %p\n", info->si_addr);
    if (ioctl(umem_fd, UMEM_IOC_PAGE_FAULT, &user_umem_info) < 0)
    {
        sigaction(SIGSEGV, &oldact, NULL);
    }
}

static void umem_user_init(void)
{
    struct sigaction act = {0};
    umem_fd = open(UMEM_PATH_NAME, O_RDONLY);
    if (umem_fd < 0)
    {
        return;
    }
    act.sa_sigaction = umem_sa_sigaction;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGSEGV);
    act.sa_flags = SA_SIGINFO | SA_RESTART;
    if (sigaction(SIGSEGV, &act, &oldact) < 0)
    {
        close(umem_fd);
        umem_fd = -1;
        return;
    }
}

void *umem_malloc(unsigned int umem_size, unsigned int umem_pool)
{
    int err;
    struct umem_info user_umem_info = {
        .umem_size = umem_size,
        .umem_pool = umem_pool,
    };
    if (umem_fd < 0)
    {
        umem_user_init();
        if (umem_fd < 0)
        {
            return NULL;
        }
    }
    err = ioctl(umem_fd, UMEM_IOC_MALLOC, &user_umem_info);
    if (err < 0)
    {
        return NULL;
    }
    return (void *)user_umem_info.umem_addr;
}

void umem_free(void *umem_addr)
{
    struct umem_info user_umem_info = {
        .umem_addr = (__u64)umem_addr,
    };
    if (umem_fd < 0)
    {
        return;
    }
    ioctl(umem_fd, UMEM_IOC_FREE, &user_umem_info);
}