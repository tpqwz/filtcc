#include <time.h>

int main(void)
{
    time_t t = 0;
    struct tm *g;
    struct tm *g2;
    struct tm *loc;
    struct tm *(*fg)(const time_t *);
    struct tm *(*fl)(const time_t *);

    g = gmtime(&t);
    if (!g || g->tm_year != 70 || g->tm_mon != 0 || g->tm_mday != 1)
        return 3;
    if (g->tm_hour != 0 || g->tm_wday != 4)
        return 4;
    g->tm_min = 7;
    if (g->tm_min != 7)
        return 5;
    g2 = gmtime(&t);
    if (g2 != g || g2->tm_year != 70 || g2->tm_min != 0)
        return 6;
    loc = localtime(&t);
    if (loc != g || loc->tm_mday < 1 || loc->tm_mday > 31)
        return 7;
    fg = gmtime;
    g = fg(&t);
    if (!g || g != g2 || g->tm_year != 70 || g->tm_mon != 0 || g->tm_mday != 1)
        return 8;
    fl = localtime;
    loc = fl(&t);
    if (!loc || loc != g || loc->tm_mday < 1 || loc->tm_mday > 31)
        return 9;
    return 0;
}
