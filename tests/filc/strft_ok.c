#include <locale.h>
#include <string.h>
#include <time.h>

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
    char buf[16];
    char slot;
    size_t n;
    size_t (*fn)(char *, size_t, const char *, const struct tm *);
    size_t (*fl)(char *, size_t, const char *, const struct tm *, locale_t);

    set_tm(&tm);
    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    n = strftime(buf, 16, "%Y-%m-%d", &tm);
    if (n != 10 || strcmp(buf, "2020-01-02") != 0)
        return 3;
    if (strftime(0, 0, "%Y", &tm) != 0)
        return 4;
    slot = 99;
    if (strftime(&slot, 0, "%Y", &tm) != 0 || slot != 99)
        return 5;
    n = strftime_l(buf, 16, "%Y", &tm, loc);
    if (n != 4 || strcmp(buf, "2020") != 0)
        return 6;
    n = strftime_l(buf, 16, "%m", &tm, LC_GLOBAL_LOCALE);
    if (n != 2 || strcmp(buf, "01") != 0)
        return 7;
    if (strftime_l(0, 0, "%Y", &tm, LC_GLOBAL_LOCALE) != 0)
        return 8;
    fn = strftime;
    n = fn(buf, 16, "%d", &tm);
    if (n != 2 || strcmp(buf, "02") != 0)
        return 9;
    fl = strftime_l;
    n = fl(buf, 16, "%Y", &tm, loc);
    if (n != 4 || strcmp(buf, "2020") != 0)
        return 10;
    n = fl(buf, 16, "%Y", &tm, LC_GLOBAL_LOCALE);
    if (n != 4 || strcmp(buf, "2020") != 0)
        return 11;
    freelocale(loc);
    return 0;
}
