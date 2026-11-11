#include <locale.h>
#include <strings.h>

int main(void)
{
    locale_t loc;
    int (*sc)(const char *, const char *, locale_t);
    int (*sn)(const char *, const char *, size_t, locale_t);
    char buf[2];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    if (strcasecmp_l("AbC", "abc", loc) != 0)
        return 3;
    if (strcasecmp_l("AbC", "abd", loc) >= 0)
        return 4;
    if (strncasecmp_l("AbZ", "abc", 2, loc) != 0)
        return 5;
    if (strncasecmp_l("AbZ", "abc", 3, loc) == 0)
        return 6;
    if (strncasecmp_l("abc", "XYZ", 0, loc) != 0)
        return 7;
    buf[0] = 'A';
    buf[1] = 'B';
    if (strncasecmp_l(buf, "zz", 0, loc) != 0)
        return 8;
    if (strcasecmp_l("AbC", "abc", LC_GLOBAL_LOCALE) != 0)
        return 9;
    if (strncasecmp_l("AbZ", "abc", 2, LC_GLOBAL_LOCALE) != 0)
        return 10;
    sc = strcasecmp_l;
    if (sc("Hi", "hi", loc) != 0)
        return 11;
    if (sc("Hi", "hi", LC_GLOBAL_LOCALE) != 0)
        return 12;
    sn = strncasecmp_l;
    if (sn("AbZ", "abc", 2, loc) != 0)
        return 13;
    if (sn(buf, "zz", 0, LC_GLOBAL_LOCALE) != 0)
        return 14;
    freelocale(loc);
    return 0;
}
