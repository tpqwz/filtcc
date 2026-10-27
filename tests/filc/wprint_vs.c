#include <stdarg.h>
#include <wchar.h>

static int wrap(wchar_t *d, size_t n, const wchar_t *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    r = vswprintf(d, n, fmt, ap);
    va_end(ap);
    return r;
}

int main(void)
{
    wchar_t buf[16];
    char b[4] = {'a', 'b', 'c', 'd'};
    wrap(buf, 16, L"%s", b);
    return 0;
}
