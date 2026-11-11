#include <locale.h>
#include <wchar.h>

extern long wcstol_l(const wchar_t *, wchar_t **, int, locale_t);

int main(void)
{
    locale_t loc;
    wchar_t b[3];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    b[0] = L'1';
    b[1] = L'2';
    b[2] = L'3';
    (void)wcstol_l(b, 0, 10, loc);
    freelocale(loc);
    return 0;
}
