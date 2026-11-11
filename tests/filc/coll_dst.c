#include <locale.h>
#include <string.h>

int main(void)
{
    locale_t loc;
    char tiny[1];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    tiny[0] = 0;
    (void)strxfrm_l(tiny, "ab", 2, loc);
    freelocale(loc);
    return 0;
}
