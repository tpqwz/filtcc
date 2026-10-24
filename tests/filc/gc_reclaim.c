#include <stdlib.h>
#include <unistd.h>
void filc_collect(void);
unsigned long filc_freed_count(void);

__attribute__((noinline)) static void leak(void)
{
    char *p = malloc(256 * 1024);
    if (!p)
        _exit(2);
    p[0] = 1;
    p[256 * 1024 - 1] = 2;
    free(p);
}

__attribute__((noinline)) static void scrub(void)
{
    volatile char buf[8192];
    int i;
    for (i = 0; i < 8192; i++)
        buf[i] = 0;
}

int main(void)
{
    char *keep = malloc(32);
    unsigned long before;
    if (!keep)
        return 2;
    keep[0] = 7;
    keep[31] = 9;
    leak();
    scrub();
    before = filc_freed_count();
    filc_collect();
    if (filc_freed_count() == before)
        return 3;
    if (keep[0] != 7 || keep[31] != 9)
        return 4;
    return 0;
}
