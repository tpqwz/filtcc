#include <locale.h>
#include <wchar.h>

int main(void)
{
    locale_t loc;
    wchar_t src[1];
    wchar_t dst[4];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    src[0] = L'a';
    (void)wcsxfrm_l(dst, src, 4, loc);
    freelocale(loc);
    return 0;
}
