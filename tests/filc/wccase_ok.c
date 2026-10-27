#include <wchar.h>

int main(void)
{
    wchar_t one;
    int (*cs)(const wchar_t *, const wchar_t *);
    int (*cn)(const wchar_t *, const wchar_t *, size_t);

    if (wcscasecmp(L"AbC", L"abc") != 0)
        return 2;
    if (wcscasecmp(L"a", L"B") >= 0 || wcscasecmp(L"B", L"a") <= 0)
        return 3;
    if (wcscasecmp(L"", L"") != 0)
        return 4;
    if (wcsncasecmp(L"AbZ", L"aby", 2) != 0)
        return 5;
    if (wcsncasecmp(L"AbZ", L"aby", 3) == 0)
        return 6;
    if (wcsncasecmp(L"Ab", L"ab", 100) != 0)
        return 7;
    one = L'A';
    if (wcsncasecmp(&one, L"zzz", 0) != 0)
        return 8;
    cs = wcscasecmp;
    cn = wcsncasecmp;
    if (cs(L"AbC", L"abc") != 0)
        return 9;
    if (cn(L"AbZ", L"aby", 2) != 0)
        return 10;
    return 0;
}
