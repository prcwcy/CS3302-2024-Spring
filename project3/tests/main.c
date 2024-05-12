#include "umem-user.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    char *p = umem_malloc(100, 1);
    char *q;
    if (p)
    {
        printf("umem_malloc ret: %p\n", p);
    }
    printf("p[1]: %d\n", p[1]);
    p[1] = 'a';
    printf("p[1]: %d\n", p[1]);
    q = umem_malloc(100, 1);
    q[5] = 'b';
    umem_free(p);
    return 0;
}