#define _GNU_SOURCE
#include <dlfcn.h>

int main(void)
{
    int x = 0;

    dlvsym(&x, "rand", "GLIBC_2.2.5");
    return x;
}
