#include <stdio.h>

int main(void)
{
    char buf[4];
    int (*sp)(char *, size_t, const char *, ...) = snprintf;
    (void)sp(buf, 8, "a");
    return 0;
}
