#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *name = malloc(4);

    if (!name)
        return 2;
    memcpy(name, "rand", 4);
    dlsym(RTLD_DEFAULT, name);
    return 0;
}
