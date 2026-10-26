#include <stdarg.h>
#include <stdio.h>

static int wrap(const char *s, va_list ap)
{
    return vsscanf(s, "%s", ap);
}

static int go(const char *s, ...)
{
    va_list ap;
    int r;
    va_start(ap, s);
    r = wrap(s, ap);
    va_end(ap);
    return r;
}

int main(void)
{
    char b[8];
    (void)go("hi", b);
    return 0;
}
