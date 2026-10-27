#include <string.h>
#include <wchar.h>

int main(void)
{
    wchar_t d[8];
    char out[8];
    char src[] = "ab";
    char raw[2];
    const char *p;
    const wchar_t *wp;
    wchar_t ws[] = L"ab";
    mbstate_t st;
    size_t r;
    size_t (*fn)(wchar_t *, const char **, size_t, size_t, mbstate_t *);

    memset(&st, 0, sizeof st);
    d[0] = d[1] = d[2] = 9;
    p = src;
    r = mbsnrtowcs(d, &p, 1, 8, &st);
    if (r != 1 || p != src + 1 || d[0] != L'a' || p[0] != 'b')
        return 1;
    r = mbsnrtowcs(d, &p, 2, 8, &st);
    if (r != 1 || p != 0 || d[0] != L'b' || d[1] != 0)
        return 2;

    memset(&st, 0, sizeof st);
    p = src;
    r = mbsnrtowcs(0, &p, 2, 0, &st);
    if (r != 2 || p != src)
        return 3;

    memset(&st, 0, sizeof st);
    d[0] = 9;
    p = src;
    r = mbsnrtowcs(d, &p, 2, 0, &st);
    if (r != 0 || p != src || d[0] != 9)
        return 4;

    memset(&st, 0, sizeof st);
    d[0] = 9;
    p = "";
    r = mbsnrtowcs(d, &p, 1, 8, &st);
    if (r != 0 || p != 0 || d[0] != 0)
        return 5;

    d[0] = d[1] = d[2] = 9;
    p = src;
    r = mbsnrtowcs(d, &p, 3, 8, 0);
    if (r != 2 || p != 0 || d[0] != L'a' || d[1] != L'b' || d[2] != 0)
        return 6;

    raw[0] = 'a';
    raw[1] = 'b';
    memset(&st, 0, sizeof st);
    d[0] = d[1] = d[2] = 9;
    p = raw;
    r = mbsnrtowcs(d, &p, 2, 8, &st);
    if (r != 2 || p != raw + 2 || d[0] != L'a' || d[1] != L'b' || d[2] != 9)
        return 7;

    memset(&st, 0, sizeof st);
    out[0] = out[1] = out[2] = 9;
    wp = ws;
    r = wcsnrtombs(out, &wp, 1, 8, &st);
    if (r != 1 || wp != ws + 1 || out[0] != 'a' || wp[0] != L'b')
        return 8;
    r = wcsnrtombs(out, &wp, 2, 8, &st);
    if (r != 1 || wp != 0 || out[0] != 'b' || out[1] != 0)
        return 9;

    memset(&st, 0, sizeof st);
    d[0] = 9;
    p = src;
    {
        char bad[2];
        bad[0] = (char)0xff;
        bad[1] = 0;
        p = bad;
        r = mbsnrtowcs(d, &p, 2, 8, &st);
        if (r != (size_t)-1 || p != bad)
            return 10;
    }

    fn = mbsnrtowcs;
    memset(&st, 0, sizeof st);
    d[0] = d[1] = 9;
    p = src;
    r = fn(d, &p, 3, 8, &st);
    if (r != 2 || p != 0 || d[0] != L'a' || d[1] != L'b')
        return 11;
    return 0;
}
