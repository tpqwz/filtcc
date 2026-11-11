#include <locale.h>
#include <wchar.h>

int main(void)
{
    locale_t loc;
    size_t (*fn)(wchar_t *, const wchar_t *, size_t, locale_t);
    wchar_t dst[4];
    wchar_t src[1];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    src[0] = L'a';
    fn = wcsxfrm_l;
    (void)fn(dst, src, 4, loc);
    freelocale(loc);
    return 0;
}
