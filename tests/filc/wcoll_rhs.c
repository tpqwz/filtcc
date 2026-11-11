#include <locale.h>
#include <wchar.h>

int main(void)
{
    locale_t loc;
    wchar_t b[1];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    b[0] = L'b';
    (void)wcscoll_l(L"a", b, loc);
    freelocale(loc);
    return 0;
}
