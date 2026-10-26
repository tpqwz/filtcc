#define _GNU_SOURCE
#include <dlfcn.h>

int main(void)
{
    dlsym((void *)1, "rand");
    return 0;
}
