#include <locale.h>
#include <wchar.h>

int main(void)
{
    locale_t loc;
    wchar_t one;
    int (*cs)(const wchar_t *, const wchar_t *, locale_t);
    int (*cn)(const wchar_t *, const wchar_t *, size_t, locale_t);

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    if (wcscasecmp_l(L"AbC", L"abc", loc) != 0)
        return 3;
    if (wcscasecmp_l(L"a", L"B", loc) >= 0 || wcscasecmp_l(L"B", L"a", loc) <= 0)
        return 4;
    if (wcscasecmp_l(L"", L"", loc) != 0)
        return 5;
    if (wcsncasecmp_l(L"AbZ", L"aby", 2, loc) != 0)
        return 6;
    if (wcsncasecmp_l(L"AbZ", L"aby", 3, loc) == 0)
        return 7;
    if (wcsncasecmp_l(L"Ab", L"ab", 100, loc) != 0)
        return 8;
    one = L'A';
    if (wcsncasecmp_l(&one, L"zzz", 0, loc) != 0)
        return 9;
    if (wcscasecmp_l(L"AbC", L"abc", LC_GLOBAL_LOCALE) != 0)
        return 10;
    if (wcsncasecmp_l(&one, L"zzz", 0, LC_GLOBAL_LOCALE) != 0)
        return 11;
    if (wcsncasecmp_l(L"AbZ", L"aby", 2, LC_GLOBAL_LOCALE) != 0)
        return 12;
    cs = wcscasecmp_l;
    cn = wcsncasecmp_l;
    if (cs(L"AbC", L"abc", loc) != 0)
        return 13;
    if (cs(L"AbC", L"abc", LC_GLOBAL_LOCALE) != 0)
        return 14;
    if (cn(L"AbZ", L"aby", 2, loc) != 0)
        return 15;
    if (cn(&one, L"zzz", 0, LC_GLOBAL_LOCALE) != 0)
        return 16;
    freelocale(loc);
    return 0;
}
