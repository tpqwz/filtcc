#include <errno.h>
#include <limits.h>
#include <stdlib.h>

int main(void)
{
    char buf[] = "42abc";
    char small[] = "Z";
    char text[] = "11 22";
    char *end;
    char *p;
    long v;
    long long ll;
    unsigned long u;
    unsigned long long uu;
    float f;
    double d;
    long double ld;
    long (*fn)(const char *, char **, int);

    /* The slot used to name a different object. The end pointer has to
     * carry the source capability, or the tail read is out of bounds. */
    end = small;
    v = strtol(buf, &end, 10);
    if (v != 42 || end != buf + 2 || end[0] != 'a' || end[2] != 'c')
        return 1;

    p = text;
    ll = strtoll(p, (char **)&p, 10);
    if (ll != 11 || *p != ' ')
        return 2;
    ll = strtoll(p, (char **)&p, 10);
    if (ll != 22 || *p != 0)
        return 3;

    if (strtol("5", 0, 10) != 5)
        return 4;
    u = strtoul("99", &end, 10);
    if (u != 99 || *end != 0)
        return 5;
    uu = strtoull("18446744073709551615", &end, 10);
    if (uu != 18446744073709551615ull || *end != 0)
        return 6;

    d = strtod("1.5e1tail", &end);
    if (d != 15.0 || *end != 't')
        return 7;
    f = strtof("2.5", &end);
    if (f != 2.5f || *end != 0)
        return 8;
    ld = strtold("3.25", &end);
    if (ld != 3.25L || *end != 0)
        return 9;

    if (atoi(" -8") != -8 || atol("9") != 9 || atoll("12") != 12)
        return 10;
    if (atof("1.25") != 1.25)
        return 11;

    errno = 0;
    v = strtol("999999999999999999999", &end, 10);
    if (v != LONG_MAX || errno != ERANGE || *end != 0)
        return 12;

    fn = strtol;
    end = small;
    v = fn("7q", &end, 10);
    if (v != 7 || *end != 'q')
        return 13;
    return 0;
}
