#include <wchar.h>

int main(void)
{
    wchar_t buf[] = L"abc123";
    wchar_t small[] = L"Z";
    wchar_t bad[1];
    wchar_t *p;
    wchar_t *(*fn)(const wchar_t *, const wchar_t *);
    size_t (*sp)(const wchar_t *, const wchar_t *);

    bad[0] = L'Z';
    if (wcsspn(L"abc123", L"cba") != 3)
        return 1;
    if (wcsspn(L"abc", L"abc") != 3)
        return 2;
    if (wcsspn(L"", L"abc") != 0)
        return 3;
    if (wcsspn(L"ab", L"") != 0)
        return 4;
    if (wcsspn(L"", bad) != 0)
        return 5;

    if (wcscspn(L"abc123", L"0123456789") != 3)
        return 6;
    if (wcscspn(L"abc", L"xyz") != 3)
        return 7;
    if (wcscspn(L"", L"a") != 0)
        return 8;
    if (wcscspn(L"ab", L"") != 2)
        return 9;
    if (wcscspn(L"", bad) != 0)
        return 10;

    p = small;
    p = wcspbrk(buf, L"321");
    if (p != buf + 3 || p[0] != L'1' || p[1] != L'2' || p[-1] != L'c')
        return 11;
    if (wcspbrk(L"abc", L"xyz") != 0)
        return 12;
    if (wcspbrk(L"", L"a") != 0)
        return 13;
    if (wcspbrk(L"ab", L"") != 0)
        return 14;
    if (wcspbrk(L"", bad) != 0)
        return 15;

    fn = wcspbrk;
    p = fn(L"qZ", L"Z");
    if (!p || p[0] != L'Z' || p[-1] != L'q' || p[1] != 0)
        return 16;
    sp = wcsspn;
    if (sp(L"ab", L"a") != 1)
        return 17;
    return 0;
}
