#include <locale.h>
#include <wchar.h>

int main(void)
{
    locale_t loc;

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    (void)wcsxfrm_l((wchar_t *)L"abcd", L"a", 2, loc);
    freelocale(loc);
    return 0;
}
