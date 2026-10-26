#define _GNU_SOURCE
#include <dlfcn.h>

int main(void)
{
    char *p = dlsym(RTLD_DEFAULT, "_libc_intl_domainname");

    if (!p)
        return 0;
    p[0] = 'X';
    return 0;
}
