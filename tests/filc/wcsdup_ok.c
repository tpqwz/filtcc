#include <wchar.h>
#include <stdlib.h>

int main(void)
{
    wchar_t src[] = L"ab";
    wchar_t *p;
    wchar_t *(*fn)(const wchar_t *);

    p = wcsdup(src);
    if (!p || p == src || p[0] != L'a' || p[1] != L'b' || p[2] != 0)
        return 1;
    p[0] = L'Z';
    if (src[0] != L'a' || p[0] != L'Z')
        return 2;
    free(p);

    p = wcsdup(L"");
    if (!p || p[0] != 0)
        return 3;
    p[0] = L'Q';
    if (p[0] != L'Q')
        return 4;
    free(p);

    fn = wcsdup;
    p = fn(L"hi");
    if (!p || p[0] != L'h' || p[1] != L'i' || p[2] != 0)
        return 5;
    p[1] = L'Z';
    if (p[0] != L'h' || p[1] != L'Z' || L"hi"[0] != L'h')
        return 6;
    free(p);
    return 0;
}
