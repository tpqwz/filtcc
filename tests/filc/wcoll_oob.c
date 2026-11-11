#include <locale.h>
#include <wchar.h>

int main(void)
{
    locale_t loc;
    wchar_t a[1];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    a[0] = L'a';
    (void)wcscoll_l(a, L"b", loc);
    freelocale(loc);
    return 0;
}
