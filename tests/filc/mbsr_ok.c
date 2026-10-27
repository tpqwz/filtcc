#include <string.h>
#include <wchar.h>

int main(void)
{
    wchar_t buf[8];
    char bytes[8];
    mbstate_t st;
    const char *p;
    const wchar_t *wp;
    size_t n;
    size_t (*fn)(wchar_t *, const char **, size_t, mbstate_t *);
    char raw[4];

    memset(&st, 0, sizeof st);
    p = "ab";
    n = mbsrtowcs(buf, &p, 8, &st);
    if (n != 2 || p != NULL || buf[0] != L'a' || buf[1] != L'b' || buf[2] != 0)
        return 1;

    memset(&st, 0, sizeof st);
    p = "ab";
    n = mbsrtowcs(buf, &p, 1, &st);
    if (n != 1 || !p || p[0] != 'b' || buf[0] != L'a')
        return 2;
    n = mbsrtowcs(buf + 1, &p, 4, &st);
    if (n != 1 || p != NULL || buf[1] != L'b' || buf[2] != 0)
        return 3;

    memset(&st, 0, sizeof st);
    p = "ab";
    n = mbsrtowcs(NULL, &p, 0, &st);
    if (n != 2 || !p || p[0] != 'a')
        return 4;

    memset(&st, 0, sizeof st);
    p = "ab";
    buf[0] = L'Z';
    n = mbsrtowcs(buf, &p, 0, &st);
    if (n != 0 || !p || p[0] != 'a' || buf[0] != L'Z')
        return 5;

    memset(&st, 0, sizeof st);
    p = "";
    n = mbsrtowcs(buf, &p, 4, &st);
    if (n != 0 || p != NULL || buf[0] != 0)
        return 6;

    p = "ab";
    n = mbsrtowcs(buf, &p, 8, NULL);
    if (n != 2 || p != NULL || buf[0] != L'a' || buf[1] != L'b')
        return 7;

    memset(&st, 0, sizeof st);
    wp = L"ab";
    n = wcsrtombs(bytes, &wp, 8, &st);
    if (n != 2 || wp != NULL || strcmp(bytes, "ab") != 0)
        return 8;

    memset(&st, 0, sizeof st);
    wp = L"ab";
    n = wcsrtombs(bytes, &wp, 1, &st);
    if (n != 1 || !wp || wp[0] != L'b' || bytes[0] != 'a')
        return 9;
    n = wcsrtombs(bytes + 1, &wp, 4, &st);
    if (n != 1 || wp != NULL || bytes[1] != 'b' || bytes[2] != 0)
        return 10;

    memset(&st, 0, sizeof st);
    wp = L"ab";
    n = wcsrtombs(NULL, &wp, 0, &st);
    if (n != 2 || !wp || wp[0] != L'a')
        return 11;

    memset(&st, 0, sizeof st);
    wp = L"ab";
    bytes[0] = 'Z';
    n = wcsrtombs(bytes, &wp, 0, &st);
    if (n != 0 || !wp || wp[0] != L'a' || bytes[0] != 'Z')
        return 12;

    memset(&st, 0, sizeof st);
    wp = L"";
    n = wcsrtombs(bytes, &wp, 4, &st);
    if (n != 0 || wp != NULL || bytes[0] != 0)
        return 13;

    wp = L"ab";
    n = wcsrtombs(bytes, &wp, 8, NULL);
    if (n != 2 || wp != NULL || strcmp(bytes, "ab") != 0)
        return 14;

    raw[0] = (char)0xff;
    raw[1] = 0;
    p = raw;
    memset(&st, 0, sizeof st);
    n = mbsrtowcs(buf, &p, 4, &st);
    if (n != (size_t)-1 || p != raw)
        return 15;

    fn = mbsrtowcs;
    memset(&st, 0, sizeof st);
    p = "xy";
    n = fn(buf, &p, 8, &st);
    if (n != 2 || p != NULL || buf[0] != L'x' || buf[1] != L'y' || buf[2] != 0)
        return 16;

    return 0;
}
