#define _GNU_SOURCE
#include <dlfcn.h>

int main(void)
{
    int x = 0;

    dlsym(&x, "rand");
    return x;
}
