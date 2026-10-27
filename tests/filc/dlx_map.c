#define _GNU_SOURCE
#include <dlfcn.h>

int main(void)
{
    Dl_info info;
    void *extra = 0;
    void *p = dlsym(RTLD_DEFAULT, "optind");
    volatile char c;

    if (!p || dladdr1(p, &info, &extra, RTLD_DL_LINKMAP) == 0 || !extra)
        return 2;
    c = *(char *)extra;
    (void)c;
    return 0;
}
