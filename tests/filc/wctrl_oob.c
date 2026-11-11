#include <locale.h>
#include <wctype.h>

int main(void)
{
    locale_t loc;
    char buf[7];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    buf[0] = 't';
    buf[1] = 'o';
    buf[2] = 'l';
    buf[3] = 'o';
    buf[4] = 'w';
    buf[5] = 'e';
    buf[6] = 'r';
    (void)wctrans_l(buf, loc);
    freelocale(loc);
    return 0;
}
