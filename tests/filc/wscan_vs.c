#include <stdarg.h>
#include <wchar.h>

static int wrap(const wchar_t *s, va_list ap)
{
    return vswscanf(s, L"%d", ap);
}

static int go(const wchar_t *s, ...)
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
    (void)go(L"1", (int *)&tiny);
    return 0;
}
