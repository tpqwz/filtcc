#include <locale.h>
#include <string.h>

int main(void)
{
    locale_t loc;
    int (*cfn)(const char *, const char *, locale_t);
    size_t (*xfn)(char *, const char *, size_t, locale_t);
    char buf[8];
    char slot;
    char a[8];
    char b[8];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    if (strcoll_l("a", "b", loc) >= 0 || strcoll_l("b", "a", loc) <= 0)
        return 3;
    if (strcoll_l("a", "a", loc) != 0 || strcoll_l("", "", loc) != 0)
        return 4;
    if (strxfrm_l(0, "ab", 0, loc) != 2)
        return 5;
    if (strxfrm_l(buf, "ab", 8, loc) != 2 || buf[0] != 'a' || buf[1] != 'b'
        || buf[2] != 0)
        return 6;
    slot = 99;
    if (strxfrm_l(&slot, "ab", 0, loc) != 2 || slot != 99)
        return 7;
    if (strxfrm_l(a, "a", 8, loc) != 1 || strxfrm_l(b, "b", 8, loc) != 1
        || strcmp(a, b) >= 0)
        return 8;
    if (strcoll_l("a", "b", LC_GLOBAL_LOCALE) >= 0)
        return 9;
    if (strxfrm_l(0, "ab", 0, LC_GLOBAL_LOCALE) != 2)
        return 10;
    slot = 99;
    if (strxfrm_l(&slot, "ab", 0, LC_GLOBAL_LOCALE) != 2 || slot != 99)
        return 11;
    if (strxfrm_l(buf, "cd", 8, LC_GLOBAL_LOCALE) != 2 || strcmp(buf, "cd") != 0)
        return 12;
    cfn = strcoll_l;
    if (cfn("a", "a", loc) != 0)
        return 13;
    if (cfn("a", "a", LC_GLOBAL_LOCALE) != 0)
        return 14;
    xfn = strxfrm_l;
    if (xfn(0, "ab", 0, loc) != 2)
        return 15;
    if (xfn(buf, "ab", 8, LC_GLOBAL_LOCALE) != 2 || strcmp(buf, "ab") != 0)
        return 16;
    freelocale(loc);
    return 0;
}
