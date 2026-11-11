#include <string.h>
#include <time.h>

int main(void)
{
    time_t t = 0;
    struct tm tm;
    struct tm loc;
    char buf[26];
    char buf2[26];
    char big[26];
    char *s;
    char *(*fa)(const struct tm *, char *);
    char *(*fc)(const time_t *, char *);

    if (gmtime_r(&t, &tm) != &tm)
        return 3;
    s = asctime_r(&tm, buf);
    if (s != buf || strcmp(s, "Thu Jan  1 00:00:00 1970\n") != 0)
        return 4;
    if (localtime_r(&t, &loc) != &loc)
        return 5;
    if (asctime_r(&loc, buf2) != buf2)
        return 6;
    s = ctime_r(&t, buf);
    if (s != buf || strcmp(s, buf2) != 0)
        return 7;
    memset(&tm, 0, sizeof tm);
    tm.tm_year = 100000 - 1900;
    tm.tm_mday = 1;
    if (asctime_r(&tm, big) != 0)
        return 8;
    if (gmtime_r(&t, &tm) != &tm)
        return 9;
    fa = asctime_r;
    s = fa(&tm, buf);
    if (s != buf || s[0] != 'T')
        return 10;
    fc = ctime_r;
    s = fc(&t, buf);
    if (s != buf || strcmp(s, buf2) != 0)
        return 11;
    return 0;
}
