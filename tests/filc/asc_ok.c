#include <string.h>
#include <time.h>

int main(void)
{
    time_t t = 0;
    struct tm *g;
    struct tm huge;
    char utc[40];
    char loc[40];
    char *s;
    char *as;
    char *(*fa)(const struct tm *);
    char *(*fc)(const time_t *);
    size_t n;

    g = gmtime(&t);
    if (!g || g->tm_year != 70)
        return 3;
    s = asctime(g);
    if (!s || strcmp(s, "Thu Jan  1 00:00:00 1970\n") != 0)
        return 4;
    memcpy(utc, s, strlen(s) + 1);
    g = localtime(&t);
    if (!g)
        return 5;
    as = asctime(g);
    if (!as)
        return 6;
    memcpy(loc, as, strlen(as) + 1);
    s = ctime(&t);
    if (s != as || strcmp(s, loc) != 0)
        return 7;
    if (asctime(0) != 0)
        return 8;
    memset(&huge, 0, sizeof huge);
    huge.tm_year = 100000 - 1900;
    huge.tm_mday = 1;
    s = asctime(&huge);
    if (!s)
        return 9;
    n = 0;
    while (s[n])
        n++;
    if (n < 26)
        return 10;
    g = gmtime(&t);
    if (!g || g->tm_year != 70)
        return 11;
    fa = asctime;
    s = fa(g);
    if (!s || strcmp(s, utc) != 0)
        return 12;
    fc = ctime;
    s = fc(&t);
    if (!s || strcmp(s, loc) != 0)
        return 13;
    return 0;
}
