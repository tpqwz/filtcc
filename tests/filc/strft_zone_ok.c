#include <string.h>
#include <time.h>
int main(void)
{
    struct tm tm;
    struct tm *p;
    char buf[16];
    char z[] = "UTC";
    char b[4] = {'a', 'b', 'c', 'd'};
    time_t t = 0;
    size_t n;
    size_t (*fn)(char *, size_t, const char *, const struct tm *);

    memset(&tm, 0, sizeof tm);
    tm.tm_year = 120;
    tm.tm_mon = 0;
    tm.tm_mday = 2;
    tm.tm_zone = z;
    n = strftime(buf, sizeof buf, "%Z", &tm);
    if (n != 3 || strcmp(buf, "UTC") != 0)
        return 2;
    tm.tm_zone = b;
    n = strftime(buf, sizeof buf, "%Y", &tm);
    if (n != 4 || strcmp(buf, "2020") != 0)
        return 3;
    n = strftime(buf, sizeof buf, "%%Z", &tm);
    if (n != 2 || strcmp(buf, "%Z") != 0)
        return 4;
    p = gmtime(&t);
    if (!p || !p->tm_zone)
        return 5;
    n = strftime(buf, sizeof buf, "%Z", p);
    if (n != strlen(p->tm_zone) || strcmp(buf, p->tm_zone) != 0)
        return 6;
    tm.tm_zone = z;
    fn = strftime;
    n = fn(buf, sizeof buf, "%Z", &tm);
    if (n != 3 || strcmp(buf, "UTC") != 0)
        return 7;
    return 0;
}
