#include <locale.h>
#include <wchar.h>

extern long wcstol_l(const wchar_t *, wchar_t **, int, locale_t);

int main(void)
{
    locale_t loc;
    long (*fn)(const wchar_t *, wchar_t **, int, locale_t);
    wchar_t b[3];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    b[0] = L'1';
    b[1] = L'2';
    b[2] = L'3';
    fn = wcstol_l;
    (void)fn(b, 0, 10, loc);
    freelocale(loc);
    return 0;
}
