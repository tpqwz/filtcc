#include <locale.h>
#include <string.h>
#include <time.h>

extern char *strptime(const char *, const char *, struct tm *);
extern char *strptime_l(const char *, const char *, struct tm *, locale_t);

int main(void)
{
    struct tm tm;
    locale_t loc;
    char buf[] = "2020-01-02X";
    char y[] = "2020Q";
    char *p;
    int year;
    char *(*fn)(const char *, const char *, struct tm *);
    char *(*fl)(const char *, const char *, struct tm *, locale_t);

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    memset(&tm, 0, sizeof tm);
    p = strptime(buf, "%Y-%m-%d", &tm);
    if (p != buf + 10 || *p != 'X')
        return 3;
    if (tm.tm_year != 120 || tm.tm_mon != 0 || tm.tm_mday != 2)
        return 4;
    *p = 'Z';
    if (buf[10] != 'Z')
        return 5;
    memset(&tm, 0x11, sizeof tm);
    year = tm.tm_year;
    if (strptime("abc", "%Y-%m-%d", &tm) != 0 || tm.tm_year != year)
        return 6;
    p = strptime("hello", "", &tm);
    if (p == 0 || *p != 'h' || tm.tm_year != year)
        return 7;
    p = strptime("hello", "", 0);
    if (p == 0 || *p != 'h')
        return 8;
    memset(&tm, 0, sizeof tm);
    tm.tm_year = 7;
    if (strptime("2020-x", "%Y-%m-%d", &tm) != 0 || tm.tm_year != 120)
        return 9;
    memset(&tm, 0, sizeof tm);
    p = strptime_l("2020-01-02", "%Y-%m-%d", &tm, loc);
    if (p == 0 || *p != 0 || tm.tm_year != 120 || tm.tm_mon != 0 || tm.tm_mday != 2)
        return 10;
    memset(&tm, 0, sizeof tm);
    p = strptime_l("2020X", "%Y", &tm, LC_GLOBAL_LOCALE);
    if (p == 0 || *p != 'X' || tm.tm_year != 120)
        return 11;
    p = strptime_l("hello", "", 0, LC_GLOBAL_LOCALE);
    if (p == 0 || *p != 'h')
        return 12;
    fn = strptime;
    memset(&tm, 0, sizeof tm);
    p = fn(y, "%Y", &tm);
    if (p != y + 4 || *p != 'Q' || tm.tm_year != 120)
        return 13;
    fl = strptime_l;
    memset(&tm, 0, sizeof tm);
    p = fl("1999", "%Y", &tm, loc);
    if (p == 0 || *p != 0 || tm.tm_year != 99)
        return 14;
    memset(&tm, 0, sizeof tm);
    p = fl("2001", "%Y", &tm, LC_GLOBAL_LOCALE);
    if (p == 0 || *p != 0 || tm.tm_year != 101)
        return 15;
    freelocale(loc);
    return 0;
}
