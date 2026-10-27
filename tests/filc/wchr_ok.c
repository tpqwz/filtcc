#include <wchar.h>

int main(void)
{
    wchar_t buf[] = L"abca";
    wchar_t small[] = L"Z";
    wchar_t *p;
    wchar_t *(*fn)(const wchar_t *, wchar_t);

    p = small;
    p = wcschr(buf, L'c');
    if (p != buf + 2 || p[0] != L'c' || p[1] != L'a')
        return 1;
    if (wcschr(buf, L'z') != 0)
        return 2;
    p = wcschr(L"ab", L'\0');
    if (!p || *p != 0)
        return 3;

    p = small;
    p = wcsrchr(buf, L'a');
    if (p != buf + 3 || p[0] != L'a' || p[-1] != L'c')
        return 4;
    if (wcsrchr(buf, L'z') != 0)
        return 5;

    p = small;
    p = wcsstr(L"xxab", L"ab");
    if (!p || p[0] != L'a' || p[1] != L'b' || p[2] != 0)
        return 6;
    p = wcsstr(buf, L"");
    if (p != buf)
        return 7;
    if (wcsstr(buf, L"z") != 0)
        return 8;

    fn = wcschr;
    p = fn(L"q", L'q');
    if (!p || p[0] != L'q' || p[1] != 0)
        return 9;
    return 0;
}
