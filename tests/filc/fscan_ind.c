#include <stdio.h>

int main(void)
{
    int (*fn)(FILE *, const char *, ...) = fscanf;
    char tiny;
    (void)fn(stdin, "%u", (unsigned *)&tiny);
    return 0;
}
