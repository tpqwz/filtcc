#define _GNU_SOURCE
#include <dlfcn.h>

int main(void)
{
    void *h = dlopen("libc.so.6", RTLD_LAZY);
    volatile char c;

    if (!h)
        return 0;
    c = *(char *)h;
    (void)c;
    return 0;
}
