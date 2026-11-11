#include <locale.h>
#include <string.h>

int main(void)
{
    locale_t loc;

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    (void)strxfrm_l((char *)"abcd", "a", 2, loc);
    freelocale(loc);
    return 0;
}
