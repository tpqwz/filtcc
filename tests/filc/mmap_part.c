#define _GNU_SOURCE
#include <sys/mman.h>

int main(void)
{
    char *p = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 0);
    if (p == MAP_FAILED)
        return 1;
    (void)munmap(p, 2048);
    return 0;
}
