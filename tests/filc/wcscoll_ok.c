#include <wchar.h>

int main(void)
{
    wchar_t buf[8];
    wchar_t slot;
    wchar_t a[8];
    wchar_t b[8];
    int (*cfn)(const wchar_t *, const wchar_t *);

    if (wcscoll(L"a", L"b") >= 0 || wcscoll(L"b", L"a") <= 0)
        return 2;
    if (wcscoll(L"a", L"a") != 0 || wcscoll(L"", L"") != 0)
        return 3;
    if (wcsxfrm(0, L"ab", 0) != 2)
        return 4;
    if (wcsxfrm(buf, L"ab", 8) != 2 || buf[0] != L'a' || buf[1] != L'b'
        || buf[2] != 0)
        return 5;
    slot = 99;
    if (wcsxfrm(&slot, L"ab", 0) != 2 || slot != 99)
        return 6;
    if (wcsxfrm(a, L"a", 8) != 1 || wcsxfrm(b, L"b", 8) != 1
        || wcscmp(a, b) >= 0)
        return 7;
    cfn = wcscoll;
    if (cfn(L"a", L"a") != 0)
        return 8;
    return 0;
}
