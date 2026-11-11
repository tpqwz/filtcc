#include <locale.h>
#include <wchar.h>

int main(void)
{
    locale_t loc;
    int (*fn)(const wchar_t *, const wchar_t *, locale_t);
    wchar_t src[1];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    src[0] = L'a';
    fn = wcscasecmp_l;
    (void)fn(src, L"a", loc);
    freelocale(loc);
    return 0;
}
