#include <locale.h>

extern long strtol_l(const char *, char **, int, locale_t);
extern long long strtoll_l(const char *, char **, int, locale_t);
extern unsigned long strtoul_l(const char *, char **, int, locale_t);
extern unsigned long long strtoull_l(const char *, char **, int, locale_t);
extern float strtof_l(const char *, char **, locale_t);
extern double strtod_l(const char *, char **, locale_t);
extern long double strtold_l(const char *, char **, locale_t);
extern long isoc_strtol_l(const char *, char **, int, locale_t)
    __asm__("__isoc23_strtol_l");

int main(void)
{
    locale_t loc;
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
    long (*fn)(const char *, char **, int, locale_t);
    long (*iso)(const char *, char **, int, locale_t);

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    end = small;
    v = strtol_l(buf, &end, 10, loc);
    if (v != 42 || end != buf + 2 || end[0] != 'a' || end[2] != 'c')
        return 3;
    p = text;
    ll = strtoll_l(p, &p, 10, loc);
    if (ll != 11 || *p != ' ')
        return 4;
    ll = strtoll_l(p, &p, 10, loc);
    if (ll != 22 || *p != 0)
        return 5;
    if (strtol_l("5", 0, 10, loc) != 5)
        return 6;
    u = strtoul_l("99", &end, 10, loc);
    if (u != 99 || *end != 0)
        return 7;
    uu = strtoull_l("18446744073709551615", &end, 10, loc);
    if (uu != 18446744073709551615ull || *end != 0)
        return 8;
    d = strtod_l("1.5e1tail", &end, loc);
    if (d != 15.0 || *end != 't')
        return 9;
    f = strtof_l("2.5", &end, loc);
    if (f != 2.5f || *end != 0)
        return 10;
    ld = strtold_l("3.25", &end, loc);
    if (ld != 3.25L || *end != 0)
        return 11;
    end = small;
    v = strtol_l(buf, &end, 10, LC_GLOBAL_LOCALE);
    if (v != 42 || end != buf + 2 || end[0] != 'a')
        return 12;
    if (strtod_l("1.5", 0, LC_GLOBAL_LOCALE) != 1.5)
        return 13;
    fn = strtol_l;
    end = small;
    v = fn("7q", &end, 10, loc);
    if (v != 7 || *end != 'q')
        return 14;
    iso = isoc_strtol_l;
    end = small;
    v = iso("8z", &end, 10, LC_GLOBAL_LOCALE);
    if (v != 8 || *end != 'z')
        return 15;
    freelocale(loc);
    return 0;
}
