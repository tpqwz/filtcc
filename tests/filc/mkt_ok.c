#include <string.h>
#include <time.h>

int main(void)
{
    struct tm tm;
    struct tm tm2;
    struct tm *loc;
    struct tm *g;
    time_t t;
    time_t u;
    time_t now;
    time_t stored;
    time_t z;
    time_t (*fm)(struct tm *);
    time_t (*ft)(time_t *);

    memset(&tm, 0, sizeof tm);
    tm.tm_year = 70;
    tm.tm_mday = 32;
    tm.tm_isdst = -1;
    t = mktime(&tm);
    if (t == (time_t)-1)
        return 3;
    if (tm.tm_year != 70 || tm.tm_mon != 1 || tm.tm_mday != 1)
        return 4;
    if (tm.tm_hour != 0 || tm.tm_min != 0 || tm.tm_sec != 0)
        return 5;
    if (tm.tm_wday != 0 || tm.tm_yday != 31)
        return 6;
    loc = localtime(&t);
    if (!loc)
        return 7;
    if (loc->tm_year != tm.tm_year || loc->tm_mon != tm.tm_mon
        || loc->tm_mday != tm.tm_mday)
        return 8;
    if (loc->tm_hour != tm.tm_hour || loc->tm_min != tm.tm_min
        || loc->tm_sec != tm.tm_sec)
        return 9;

    memset(&tm2, 0, sizeof tm2);
    tm2.tm_year = 70;
    tm2.tm_mday = 32;
    tm2.tm_isdst = -1;
    u = timelocal(&tm2);
    if (u != t || tm2.tm_mon != 1 || tm2.tm_mday != 1 || tm2.tm_wday != 0)
        return 10;

    now = time(0);
    if (now == (time_t)-1)
        return 11;
    stored = 0;
    if (time(&stored) != stored || stored == 0)
        return 12;
    if (stored < now || stored - now > 5)
        return 13;

    fm = mktime;
    memset(&tm2, 0, sizeof tm2);
    tm2.tm_year = 70;
    tm2.tm_mday = 1;
    tm2.tm_isdst = -1;
    u = fm(&tm2);
    if (u == (time_t)-1 || tm2.tm_year != 70 || tm2.tm_mday != 1
        || tm2.tm_wday != 4)
        return 14;
    ft = time;
    stored = 0;
    u = ft(&stored);
    if (u != stored || stored == 0)
        return 15;

    z = 0;
    g = gmtime(&z);
    if (!g || g->tm_year != 70 || g->tm_mon != 0 || g->tm_mday != 1)
        return 16;
    u = mktime(g);
    if (u == (time_t)-1 || g->tm_year != 70 || g->tm_mon != 0
        || g->tm_mday != 1 || g->tm_wday != 4)
        return 17;
    return 0;
}
