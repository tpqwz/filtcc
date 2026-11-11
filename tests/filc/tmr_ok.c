#include <string.h>
#include <time.h>

int main(void)
{
    time_t t = 0;
    struct tm tm;
    struct tm tm2;
    struct tm *r;
    struct tm *(*fg)(const time_t *, struct tm *);
    struct tm *(*fl)(const time_t *, struct tm *);

    memset(&tm, 0x11, sizeof tm);
    r = gmtime_r(&t, &tm);
    if (r != &tm || r->tm_year != 70 || tm.tm_mon != 0 || tm.tm_mday != 1)
        return 3;
    if (tm.tm_hour != 0 || tm.tm_min != 0 || tm.tm_sec != 0 || tm.tm_wday != 4)
        return 4;
    if (gmtime_r(&t, 0) != 0)
        return 5;
    r = localtime_r(&t, &tm2);
    if (r != &tm2)
        return 6;
    fg = gmtime_r;
    memset(&tm, 0, sizeof tm);
    r = fg(&t, &tm);
    if (r != &tm || r->tm_year != 70)
        return 7;
    fl = localtime_r;
    r = fl(&t, &tm2);
    if (r != &tm2)
        return 8;
    return 0;
}
