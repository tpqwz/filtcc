#define _GNU_SOURCE
#include <dlfcn.h>

int main(void)
{
    Dl_info info;
    void *p = dlsym(RTLD_DEFAULT, "optind");

    if (!p || dladdr(p, &info) == 0 || !info.dli_sname)
        return 2;
    ((char *)info.dli_sname)[0] = 'X';
    return 0;
}
