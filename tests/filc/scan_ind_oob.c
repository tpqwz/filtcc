#include <stdlib.h>
#include <string.h>

int main(void)
{
    void *(*mc)(void *, const void *, int, unsigned long) = memccpy;
    char *d = malloc(4);
    if (!d)
        return 2;
    /* 'Z' is not in the source, so all 8 bytes are read and written. */
    mc(d, "01234567", 'Z', 8);
    return 0;
}
