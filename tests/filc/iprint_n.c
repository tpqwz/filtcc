#include <stdio.h>

int main(void)
{
    int (*fn)(char *, const char *, ...) = sprintf;
    char buf[16];
    char tiny[1];
    (void)fn(buf, "%n", tiny);
    return 0;
}
