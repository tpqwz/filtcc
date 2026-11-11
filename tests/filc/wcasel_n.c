#include <locale.h>
#include <wchar.h>

int main(void)
{
    locale_t loc;
    wchar_t src[1];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    src[0] = L'A';
    (void)wcsncasecmp_l(src, L"a", 2, loc);
    freelocale(loc);
    return 0;
}
