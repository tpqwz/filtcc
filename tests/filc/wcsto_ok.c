#include <errno.h>
#include <limits.h>
#include <wchar.h>

int main(void)
{
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
    long (*fn)(const wchar_t *, wchar_t **, int);

    /* The slot used to name a different object. The end pointer has to
     * carry the source capability, or the tail read is out of bounds. */
    end = small;
    v = wcstol(buf, &end, 10);
    if (v != 42 || end != buf + 2 || end[0] != L'a' || end[2] != L'c')
        return 1;

    p = text;
    ll = wcstoll(p, &p, 10);
    if (ll != 11 || *p != L' ')
        return 2;
    ll = wcstoll(p, &p, 10);
    if (ll != 22 || *p != 0)
        return 3;

    if (wcstol(L"5", 0, 10) != 5)
        return 4;
    u = wcstoul(L"99", &end, 10);
    if (u != 99 || *end != 0)
        return 5;
    uu = wcstoull(L"18446744073709551615", &end, 10);
    if (uu != 18446744073709551615ull || *end != 0)
        return 6;

    d = wcstod(L"1.5e1tail", &end);
    if (d != 15.0 || *end != L't')
        return 7;
    f = wcstof(L"2.5", &end);
    if (f != 2.5f || *end != 0)
        return 8;
    ld = wcstold(L"3.25", &end);
    if (ld != 3.25L || *end != 0)
        return 9;

    errno = 0;
    v = wcstol(L"999999999999999999999", &end, 10);
    if (v != LONG_MAX || errno != ERANGE || *end != 0)
        return 10;

    fn = wcstol;
    end = small;
    v = fn(L"7q", &end, 10);
    if (v != 7 || *end != L'q')
        return 11;
    return 0;
}
