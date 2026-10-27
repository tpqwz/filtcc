#define _GNU_SOURCE
#include <dlfcn.h>

int main(void)
{
    char buf[8];
    void *p = dlsym(RTLD_DEFAULT, "optind");

    if (!p)
        return 2;
    dladdr(p, (Dl_info *)buf);
    return 0;
}
