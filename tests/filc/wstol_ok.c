#include <locale.h>
#include <wchar.h>

extern long wcstol_l(const wchar_t *, wchar_t **, int, locale_t);
extern long long wcstoll_l(const wchar_t *, wchar_t **, int, locale_t);
extern unsigned long wcstoul_l(const wchar_t *, wchar_t **, int, locale_t);
extern unsigned long long wcstoull_l(const wchar_t *, wchar_t **, int, locale_t);
extern float wcstof_l(const wchar_t *, wchar_t **, locale_t);
extern double wcstod_l(const wchar_t *, wchar_t **, locale_t);
extern long double wcstold_l(const wchar_t *, wchar_t **, locale_t);
extern long isoc_wcstol_l(const wchar_t *, wchar_t **, int, locale_t)
    __asm__("__isoc23_wcstol_l");

int main(void)
{
    locale_t loc;
    wchar_t buf[] = L"42abc";
    wchar_t small[] = L"Z";
    wchar_t text[] = L"11 22";
    wchar_t *end;
    wchar_t *p;
    long v;
    long long ll;
    unsigned long u;
    unsigned long long uu;
    float f;
    double d;
    long double ld;
    long (*fn)(const wchar_t *, wchar_t **, int, locale_t);
    long (*iso)(const wchar_t *, wchar_t **, int, locale_t);

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    end = small;
    v = wcstol_l(buf, &end, 10, loc);
    if (v != 42 || end != buf + 2 || end[0] != L'a' || end[2] != L'c')
        return 3;
    p = text;
    ll = wcstoll_l(p, &p, 10, loc);
    if (ll != 11 || *p != L' ')
        return 4;
    ll = wcstoll_l(p, &p, 10, loc);
    if (ll != 22 || *p != 0)
        return 5;
    if (wcstol_l(L"5", 0, 10, loc) != 5)
        return 6;
    u = wcstoul_l(L"99", &end, 10, loc);
    if (u != 99 || *end != 0)
        return 7;
    uu = wcstoull_l(L"18446744073709551615", &end, 10, loc);
    if (uu != 18446744073709551615ull || *end != 0)
        return 8;
    d = wcstod_l(L"1.5e1tail", &end, loc);
    if (d != 15.0 || *end != L't')
        return 9;
    f = wcstof_l(L"2.5", &end, loc);
    if (f != 2.5f || *end != 0)
        return 10;
    ld = wcstold_l(L"3.25", &end, loc);
    if (ld != 3.25L || *end != 0)
        return 11;
    end = small;
    v = wcstol_l(buf, &end, 10, LC_GLOBAL_LOCALE);
    if (v != 42 || end != buf + 2 || end[0] != L'a')
        return 12;
    if (wcstod_l(L"1.5", 0, LC_GLOBAL_LOCALE) != 1.5)
        return 13;
    fn = wcstol_l;
    end = small;
    v = fn(L"7q", &end, 10, loc);
    if (v != 7 || *end != L'q')
        return 14;
    iso = isoc_wcstol_l;
    end = small;
    v = iso(L"8z", &end, 10, LC_GLOBAL_LOCALE);
    if (v != 8 || *end != L'z')
        return 15;
    freelocale(loc);
    return 0;
}
