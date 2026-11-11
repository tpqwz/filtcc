#include <locale.h>
#include <wchar.h>

int main(void)
{
    locale_t loc;

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    (void)wcscasecmp_l(0, L"a", loc);
    freelocale(loc);
    return 0;
}
