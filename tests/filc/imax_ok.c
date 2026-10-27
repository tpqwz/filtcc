#include <errno.h>
#include <inttypes.h>
#include <wchar.h>

int main(void)
{
    char buf[] = "42abc";
    char small[] = "Z";
    char *end;
    wchar_t wbuf[] = L"42abc";
    wchar_t wsmall[] = L"Z";
    wchar_t *wend;
    intmax_t v;
    uintmax_t u;
    intmax_t (*fn)(const char *, char **, int);

    end = small;
    v = strtoimax(buf, &end, 10);
    if (v != 42 || end != buf + 2 || end[0] != 'a' || end[2] != 'c')
        return 1;

    if (strtoimax("5", 0, 10) != 5)
        return 2;

    u = strtoumax("18446744073709551615", &end, 10);
    if (u != UINTMAX_MAX || *end != 0)
        return 3;

    errno = 0;
    v = strtoimax("999999999999999999999", &end, 10);
    if (v != INTMAX_MAX || errno != ERANGE || *end != 0)
        return 4;

    wend = wsmall;
    v = wcstoimax(wbuf, &wend, 10);
    if (v != 42 || wend != wbuf + 2 || wend[0] != L'a' || wend[2] != L'c')
        return 5;

    u = wcstoumax(L"99", &wend, 10);
    if (u != 99 || *wend != 0)
        return 6;

    fn = strtoimax;
    end = small;
    v = fn("7q", &end, 10);
    if (v != 7 || *end != 'q' || end[1] != 0)
        return 7;
    return 0;
}
