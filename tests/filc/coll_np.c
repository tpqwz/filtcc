#include <locale.h>
#include <string.h>

int main(void)
{
    locale_t loc;

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    (void)strcoll_l(0, "a", loc);
    freelocale(loc);
    return 0;
}
