#include <locale.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

extern size_t wcsftime_l(wchar_t *, size_t, const wchar_t *,
    const struct tm *, locale_t);

int main(void)
{
    struct tm tm;
    locale_t loc;
    wchar_t buf[8];
    wchar_t fmt[2];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    memset(&tm, 0, sizeof tm);
    fmt[0] = L'%';
    fmt[1] = L'Y';
    (void)wcsftime_l(buf, 8, fmt, &tm, loc);
    freelocale(loc);
    return 0;
}
