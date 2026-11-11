#include <locale.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

extern size_t wcsftime_l(wchar_t *, size_t, const wchar_t *,
    const struct tm *, locale_t);

static void set_tm(struct tm *tm)
{
    memset(tm, 0, sizeof *tm);
    tm->tm_year = 120;
    tm->tm_mon = 0;
    tm->tm_mday = 2;
}

int main(void)
{
    struct tm tm;
    locale_t loc;
    wchar_t buf[16];
    wchar_t slot;
    size_t n;
    size_t (*fn)(wchar_t *, size_t, const wchar_t *, const struct tm *);
    size_t (*fl)(wchar_t *, size_t, const wchar_t *, const struct tm *,
        locale_t);

    set_tm(&tm);
    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    n = wcsftime(buf, 16, L"%Y-%m-%d", &tm);
    if (n != 10 || wcscmp(buf, L"2020-01-02") != 0)
        return 3;
    if (wcsftime(0, 0, L"%Y", &tm) != 0)
        return 4;
    slot = 99;
    if (wcsftime(&slot, 0, L"%Y", &tm) != 0 || slot != 99)
        return 5;
    n = wcsftime_l(buf, 16, L"%Y", &tm, loc);
    if (n != 4 || wcscmp(buf, L"2020") != 0)
        return 6;
    n = wcsftime_l(buf, 16, L"%m", &tm, LC_GLOBAL_LOCALE);
    if (n != 2 || wcscmp(buf, L"01") != 0)
        return 7;
    if (wcsftime_l(0, 0, L"%Y", &tm, LC_GLOBAL_LOCALE) != 0)
        return 8;
    fn = wcsftime;
    n = fn(buf, 16, L"%d", &tm);
    if (n != 2 || wcscmp(buf, L"02") != 0)
        return 9;
    fl = wcsftime_l;
    n = fl(buf, 16, L"%Y", &tm, loc);
    if (n != 4 || wcscmp(buf, L"2020") != 0)
        return 10;
    n = fl(buf, 16, L"%Y", &tm, LC_GLOBAL_LOCALE);
    if (n != 4 || wcscmp(buf, L"2020") != 0)
        return 11;
    freelocale(loc);
    return 0;
}
