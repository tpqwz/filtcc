#include <locale.h>
#include <wchar.h>

int main(void)
{
    locale_t loc;
    wchar_t tiny[1];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    tiny[0] = 0;
    (void)wcsxfrm_l(tiny, L"ab", 2, loc);
    freelocale(loc);
    return 0;
}
