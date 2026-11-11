#include <string.h>
#include <time.h>
int main(void)
{
    time_t t = 0;
    struct tm tm;
    struct tm *p;
    p = gmtime(&t);
    if (!p || !p->tm_zone)
        return 2;
    if (strlen(p->tm_zone) < 1)
        return 3;
    if (!gmtime_r(&t, &tm) || !tm.tm_zone)
        return 4;
    if (strlen(tm.tm_zone) < 1)
        return 5;
    if (!localtime_r(&t, &tm) || !tm.tm_zone)
        return 6;
    if (strlen(tm.tm_zone) < 1)
        return 7;
    memset(&tm, 0, sizeof tm);
    tm.tm_year = 70;
    tm.tm_mday = 1;
    if (timegm(&tm) == (time_t)-1 || !tm.tm_zone)
        return 8;
    if (strlen(tm.tm_zone) < 1)
        return 9;
    memset(&tm, 0, sizeof tm);
    tm.tm_year = 70;
    tm.tm_mday = 1;
    if (mktime(&tm) == (time_t)-1 || !tm.tm_zone)
        return 10;
    if (strlen(tm.tm_zone) < 1)
        return 11;
    return 0;
}
