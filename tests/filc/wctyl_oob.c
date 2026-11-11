#include <locale.h>
#include <wctype.h>

int main(void)
{
    locale_t loc;
    char buf[4];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    buf[0] = 'a';
    buf[1] = 'l';
    buf[2] = 'p';
    buf[3] = 'h';
    (void)wctype_l(buf, loc);
    freelocale(loc);
    return 0;
}
