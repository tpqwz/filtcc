#include <string.h>
#include <time.h>

int main(void)
{
    struct tm tm;
    struct tm *g;
    time_t t;
    time_t u;
    time_t z;
    time_t (*fg)(struct tm *);

    memset(&tm, 0, sizeof tm);
    tm.tm_year = 70;
    tm.tm_mday = 32;
    tm.tm_isdst = -1;
    t = timegm(&tm);
    if (t != (time_t)2678400)
        return 3;
    if (tm.tm_year != 70 || tm.tm_mon != 1 || tm.tm_mday != 1)
        return 4;
    if (tm.tm_hour != 0 || tm.tm_min != 0 || tm.tm_sec != 0)
        return 5;
    if (tm.tm_wday != 0 || tm.tm_yday != 31)
        return 6;
    g = gmtime(&t);
    if (!g || g->tm_year != 70 || g->tm_mon != 1 || g->tm_mday != 1
        || g->tm_hour != 0)
        return 7;

    memset(&tm, 0, sizeof tm);
    tm.tm_year = 70;
    tm.tm_mday = 1;
    tm.tm_isdst = -1;
    if (timegm(&tm) != 0 || tm.tm_wday != 4 || tm.tm_yday != 0)
        return 8;

    fg = timegm;
    memset(&tm, 0, sizeof tm);
    tm.tm_year = 70;
    tm.tm_mday = 1;
    tm.tm_isdst = -1;
    u = fg(&tm);
    if (u != 0 || tm.tm_year != 70 || tm.tm_mday != 1 || tm.tm_wday != 4)
        return 9;

    z = 0;
    g = gmtime(&z);
    if (!g || g->tm_year != 70 || g->tm_mon != 0 || g->tm_mday != 1)
        return 10;
    if (timegm(g) != 0 || g->tm_year != 70 || g->tm_mon != 0
        || g->tm_mday != 1 || g->tm_wday != 4)
        return 11;
    return 0;
}
