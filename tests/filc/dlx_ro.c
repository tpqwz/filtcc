#define _GNU_SOURCE
#include <dlfcn.h>

static const Dl_info info;

int main(void)
{
    void *p = dlsym(RTLD_DEFAULT, "optind");

    if (!p)
        return 2;
    dladdr(p, (Dl_info *)&info);
    return 0;
}
