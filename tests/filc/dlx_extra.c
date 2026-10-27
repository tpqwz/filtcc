#define _GNU_SOURCE
#include <dlfcn.h>

int main(void)
{
    Dl_info info;
    char tiny[4];
    void *p = dlsym(RTLD_DEFAULT, "optind");

    if (!p)
        return 2;
    dladdr1(p, &info, (void **)tiny, RTLD_DL_SYMENT);
    return 0;
}
