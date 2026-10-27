#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

static int wrap_sw(const wchar_t *s, const wchar_t *fmt, va_list ap)
{
    return vswscanf(s, fmt, ap);
}

static int call_sw(const wchar_t *s, const wchar_t *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    r = wrap_sw(s, fmt, ap);
    va_end(ap);
    return r;
}

static int wrap_fw(FILE *fp, const wchar_t *fmt, va_list ap)
{
    return vfwscanf(fp, fmt, ap);
}

static int call_fw(FILE *fp, const wchar_t *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    r = wrap_fw(fp, fmt, ap);
    va_end(ap);
    return r;
}

int main(void)
{
    int x, n, y;
    wchar_t w[4];
    wchar_t wc;
    char b[4];
    FILE *fp;
    int (*fn)(const wchar_t *, const wchar_t *, ...);
    int (*ff)(FILE *, const wchar_t *, ...);

    x = -1;
    n = -1;
    wc = 0;
    w[0] = w[1] = w[2] = 1;
    b[0] = b[1] = b[2] = 1;
    if (swscanf(L"42 hi Q ab 7", L"%d %2ls %lc %2s %n", &x, w, &wc, b, &n) != 4)
        return 1;
    if (x != 42 || w[0] != L'h' || w[1] != L'i' || w[2] != 0)
        return 2;
    if (wc != L'Q' || b[0] != 'a' || b[1] != 'b' || b[2] != 0 || n != 11)
        return 3;

    fp = tmpfile();
    if (!fp)
        return 4;
    if (fwprintf(fp, L"17 23 29\n") < 0)
        return 5;
    rewind(fp);
    x = 0;
    if (fwscanf(fp, L"%d", &x) != 1 || x != 17)
        return 6;
    y = 0;
    if (call_fw(fp, L"%d", &y) != 1 || y != 23)
        return 7;
    ff = fwscanf;
    x = 0;
    if (ff(fp, L"%d", &x) != 1 || x != 29)
        return 8;
    fclose(fp);

    x = 0;
    if (call_sw(L"99", L"%d", &x) != 1 || x != 99)
        return 9;
    fn = swscanf;
    x = 0;
    if (fn(L"7q", L"%d", &x) != 1 || x != 7)
        return 10;
    return 0;
}
