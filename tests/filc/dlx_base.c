#define _GNU_SOURCE
#include <dlfcn.h>

int main(void)
{
    Dl_info info;
    unsigned char *b;
    void *p = dlsym(RTLD_DEFAULT, "optind");

    if (!p || dladdr(p, &info) == 0 || !info.dli_fbase)
        return 2;
    b = info.dli_fbase;
    b[0] = 0;
    return 0;
}
