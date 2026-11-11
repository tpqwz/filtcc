#include <locale.h>
#include <string.h>

int main(void)
{
    locale_t loc;
    char src[1];
    char dst[4];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    src[0] = 'a';
    (void)strxfrm_l(dst, src, 4, loc);
    freelocale(loc);
    return 0;
}
