#define _GNU_SOURCE
#include <dlfcn.h>

int main(void)
{
    char *p = dlsym(RTLD_DEFAULT, "rand");
    volatile char c;

    if (!p)
        return 0;
    c = *p;
    (void)c;
    return 0;
}
