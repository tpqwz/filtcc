#define _GNU_SOURCE
#include <dlfcn.h>

int main(void)
{
    char *p = dlsym(RTLD_DEFAULT, "optind");
    volatile char c;

    if (!p)
        return 0;
    c = p[4];
    (void)c;
    return 0;
}
