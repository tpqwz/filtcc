#include <locale.h>
#include <wchar.h>

extern long wcstol_l(const wchar_t *, wchar_t **, int, locale_t);

int main(void)
{
    locale_t loc;

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    (void)wcstol_l(0, 0, 10, loc);
    freelocale(loc);
    return 0;
}
