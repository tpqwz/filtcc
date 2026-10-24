#include <stdarg.h>
#include <stdio.h>

static int boom(char *dst, const char *fmt, ...)
{
    va_list ap;
    int n;
    va_start(ap, fmt);
    n = vsprintf(dst, fmt, ap);
    va_end(ap);
    return n;
}

int main(void)
{
    char buf[4];
    (void)boom(buf, "hello");
    return 0;
}
