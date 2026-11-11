#include <locale.h>
#include <string.h>

int main(void)
{
    locale_t loc;
    char b[3];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    b[0] = 'a';
    b[1] = 'b';
    b[2] = 'c';
    (void)strcoll_l("abc", b, loc);
    freelocale(loc);
    return 0;
}
