#include <locale.h>
#include <wctype.h>

int main(void)
{
    locale_t loc;

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    (void)wctrans_l(0, loc);
    freelocale(loc);
    return 0;
}
