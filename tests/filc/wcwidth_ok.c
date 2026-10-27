#define _XOPEN_SOURCE 700
#include <wchar.h>

int main(void)
{
    wchar_t one[1];
    int (*w)(wchar_t);
    int (*sw)(const wchar_t *, size_t);
    const wchar_t *np = 0;

    if (wcwidth(L'a') != 1 || wcwidth(0) != 0 || wcwidth(L'\n') != -1)
        return 2;
    if (wcswidth(L"ab", 2) != 2 || wcswidth(L"ab", 1) != 1)
        return 3;
    if (wcswidth(L"a", 5) != 1 || wcswidth(L"", 1) != 0)
        return 4;
    if (wcswidth(L"a\n", 2) != -1 || wcswidth(L"ab", 0) != 0)
        return 5;
    if (wcswidth(np, 0) != 0)
        return 6;
    one[0] = L'Z';
    if (wcswidth(one, 1) != 1)
        return 7;
    w = wcwidth;
    sw = wcswidth;
    if (w(L'Q') != 1 || sw(L"Q", 4) != 1)
        return 8;
    return 0;
}
