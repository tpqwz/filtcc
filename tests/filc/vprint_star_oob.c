#include <stdarg.h>
#include <stdio.h>
static int wrap(char *dst, const char *fmt, ...)
{
    va_list ap;
    int n;
    va_start(ap, fmt);
    n = vsprintf(dst, fmt, ap);
    va_end(ap);
    return n;
}
int main(void) {
    char buf[8];
    char b[4] = {'a', 'b', 'c', 'd'};
    (void)wrap(buf, "%.*s", 5, b);
    return 0;
}
