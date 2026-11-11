#include <locale.h>
#include <string.h>

int main(void)
{
    locale_t loc;
    char a[3];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    a[0] = 'a';
    a[1] = 'b';
    a[2] = 'c';
    (void)strcoll_l(a, "abc", loc);
    freelocale(loc);
    return 0;
}
