#define _GNU_SOURCE
#include <sys/mman.h>

int main(void)
{
    char *p = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 0);
    if (p == MAP_FAILED)
        return 1;
    if (mprotect(p, 4096, PROT_READ) != 0)
        return 2;
    p[0] = 1;
    return 0;
}
