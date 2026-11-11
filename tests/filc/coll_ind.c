#include <locale.h>
#include <string.h>

int main(void)
{
    locale_t loc;
    size_t (*fn)(char *, const char *, size_t, locale_t);
    char dst[4];
    char src[1];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    src[0] = 'a';
    fn = strxfrm_l;
    (void)fn(dst, src, 4, loc);
    freelocale(loc);
    return 0;
}
