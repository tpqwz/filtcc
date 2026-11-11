#include <locale.h>
#include <wchar.h>

int main(void)
{
    locale_t loc;
    int (*cfn)(const wchar_t *, const wchar_t *, locale_t);
    size_t (*xfn)(wchar_t *, const wchar_t *, size_t, locale_t);
    wchar_t buf[8];
    wchar_t slot;
    wchar_t a[8];
    wchar_t b[8];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    if (wcscoll_l(L"a", L"b", loc) >= 0 || wcscoll_l(L"b", L"a", loc) <= 0)
        return 3;
    if (wcscoll_l(L"a", L"a", loc) != 0 || wcscoll_l(L"", L"", loc) != 0)
        return 4;
    if (wcsxfrm_l(0, L"ab", 0, loc) != 2)
        return 5;
    if (wcsxfrm_l(buf, L"ab", 8, loc) != 2 || buf[0] != L'a' || buf[1] != L'b'
        || buf[2] != 0)
        return 6;
    slot = 99;
    if (wcsxfrm_l(&slot, L"ab", 0, loc) != 2 || slot != 99)
        return 7;
    if (wcsxfrm_l(a, L"a", 8, loc) != 1 || wcsxfrm_l(b, L"b", 8, loc) != 1
        || wcscmp(a, b) >= 0)
        return 8;
    if (wcscoll_l(L"a", L"b", LC_GLOBAL_LOCALE) >= 0)
        return 9;
    if (wcsxfrm_l(0, L"ab", 0, LC_GLOBAL_LOCALE) != 2)
        return 10;
    slot = 99;
    if (wcsxfrm_l(&slot, L"ab", 0, LC_GLOBAL_LOCALE) != 2 || slot != 99)
        return 11;
    if (wcsxfrm_l(buf, L"cd", 8, LC_GLOBAL_LOCALE) != 2 || wcscmp(buf, L"cd") != 0)
        return 12;
    cfn = wcscoll_l;
    if (cfn(L"a", L"a", loc) != 0)
        return 13;
    if (cfn(L"a", L"a", LC_GLOBAL_LOCALE) != 0)
        return 14;
    xfn = wcsxfrm_l;
    if (xfn(0, L"ab", 0, loc) != 2)
        return 15;
    if (xfn(buf, L"ab", 8, LC_GLOBAL_LOCALE) != 2 || wcscmp(buf, L"ab") != 0)
        return 16;
    freelocale(loc);
    return 0;
}
