#include <stdarg.h>
#include <stdio.h>

static int wrap(const char *s, va_list ap)
{
    return vsscanf(s, "%u", ap);
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
    char tiny;
    (void)go("1", (unsigned *)&tiny);
    return 0;
}
