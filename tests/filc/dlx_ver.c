#define _GNU_SOURCE
#include <dlfcn.h>
#include <string.h>

int main(void)
{
    char ver[4];

    memcpy(ver, "GLIB", 4);
    dlvsym(RTLD_DEFAULT, "rand", ver);
    return 0;
}
