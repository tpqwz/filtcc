#include <stdio.h>

int main(void)
{
    int (*fn)(const char *, ...) = printf;
    char b[4] = {'a', 'b', 'c', 'd'};
    (void)fn("%s", b);
    return 0;
}
