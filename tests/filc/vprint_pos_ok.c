#include <stdarg.h>
#include <stdio.h>
#include <string.h>
static int wrap(char *dst, const char *fmt, ...)
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
    char buf[32];
    char b[4] = {'a', 'b', 'c', 'd'};
    int n = 0;
    int r;
    r = wrap(buf, "%2$s %1$d", 7, "ok");
    if (r != 4 || strcmp(buf, "ok 7") != 0)
        return 2;
    r = wrap(buf, "%2$s %1$f", 1.5, "ok");
    if (r != 11 || strcmp(buf, "ok 1.500000") != 0)
        return 3;
    r = wrap(buf, "%2$.*1$s", 3, b);
    if (r != 3 || memcmp(buf, "abc", 3) != 0)
        return 4;
    r = wrap(buf, "ab%2$n%1$s", "z", &n);
    if (r != 3 || n != 2 || strcmp(buf, "abz") != 0)
        return 5;
    r = wrap(buf, "%5$s", "a", "b", "c", "d", "ok");
    if (r != 2 || strcmp(buf, "ok") != 0)
        return 6;
    return 0;
}
