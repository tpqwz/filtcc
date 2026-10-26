#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *fmt;
    unsigned x;
    fmt = malloc(2);
    if (!fmt)
        return 1;
    memcpy(fmt, "%d", 2);
    (void)fscanf(stdin, fmt, &x);
    return 0;
}
