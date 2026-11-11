#include <locale.h>
#include <wctype.h>

int main(void)
{
    locale_t loc;
    wctrans_t (*fn)(const char *, locale_t);
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
    fn = wctrans_l;
    (void)fn(buf, loc);
    freelocale(loc);
    return 0;
}
