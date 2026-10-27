#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

static int wrap_vsw(wchar_t *d, size_t n, const wchar_t *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    r = vswprintf(d, n, fmt, ap);
    va_end(ap);
    return r;
}

static int wrap_vfw(FILE *fp, const wchar_t *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    r = vfwprintf(fp, fmt, ap);
    va_end(ap);
    return r;
}

int main(void)
{
    wchar_t buf[16];
    wchar_t one[1];
    char raw[4];
    wchar_t got[8];
    int n;
    int (*sw)(wchar_t *, size_t, const wchar_t *, ...) = swprintf;
    FILE *fp;

    if (swprintf(buf, 16, L"%s%ls", "x", L"y") != 2 || wcscmp(buf, L"xy") != 0)
        return 1;
    one[0] = L'Z';
    if (swprintf(buf, 16, L"%.1ls", one) != 1 || buf[0] != L'Z' || buf[1] != 0)
        return 2;
    if (swprintf(buf, 16, L"%.0ls%s", one, "z") != 1 || wcscmp(buf, L"z") != 0)
        return 3;
    if (swprintf(buf, 16, L"%lc%s", (wint_t)L'Q', "z") != 2 || wcscmp(buf, L"Qz") != 0)
        return 4;
    if (swprintf(buf, 16, L"%S", L"ab") != 2 || wcscmp(buf, L"ab") != 0)
        return 5;
    raw[0] = 'a';
    raw[1] = 'b';
    raw[2] = 'c';
    raw[3] = 'd';
    if (swprintf(buf, 16, L"%.4s", raw) != 4 || wcscmp(buf, L"abcd") != 0)
        return 6;
    n = 0;
    if (swprintf(buf, 16, L"ab%n", &n) != 2 || n != 2 || wcscmp(buf, L"ab") != 0)
        return 7;
    if (swprintf(NULL, 0, L"hi") != -1)
        return 8;
    if (wrap_vsw(buf, 16, L"%s", "cd") != 2 || wcscmp(buf, L"cd") != 0)
        return 9;
    if (sw(buf, 16, L"%ls", L"ef") != 2 || wcscmp(buf, L"ef") != 0)
        return 10;

    fp = tmpfile();
    if (!fp)
        return 11;
    if (fwprintf(fp, L"%s", "ab") != 2)
        return 12;
    if (wrap_vfw(fp, L"%ls", L"cd") != 2)
        return 13;
    rewind(fp);
    if (!fgetws(got, 8, fp) || wcscmp(got, L"abcd") != 0)
        return 14;
    fclose(fp);
    return 0;
}
