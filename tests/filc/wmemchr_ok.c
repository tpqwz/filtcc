#include <wchar.h>

int main(void)
{
    wchar_t buf[] = L"abca";
    wchar_t small[] = L"Z";
    wchar_t *p;
    wchar_t *(*fn)(const wchar_t *, wchar_t, size_t);

    p = small;
    p = wmemchr(buf, L'c', 4);
    if (p != buf + 2 || p[0] != L'c' || p[-1] != L'b' || p[1] != L'a')
        return 1;
    if (wmemchr(buf, L'z', 4) != 0)
        return 2;
    if (wmemchr(buf, L'a', 0) != 0)
        return 3;
    p = wmemchr(buf, L'\0', 5);
    if (p != buf + 4 || *p != 0)
        return 4;

    fn = wmemchr;
    p = fn(L"qZ", L'Z', 2);
    if (!p || p[0] != L'Z' || p[-1] != L'q' || p[1] != 0)
        return 5;
    return 0;
}
