#include <stdio.h>
int main(void)
{
    int (*sp)(char *, const char *, ...) = sprintf;
    char buf[16];
    char b[4] = {'a', 'b', 'c', 'd'};
    (void)sp(buf, "%2$s", "no", b);
    return 0;
}
