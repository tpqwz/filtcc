#include <locale.h>
#include <wchar.h>
#include <wctype.h>

static int nz(int v)
{
    return v != 0;
}

int main(void)
{
    locale_t loc;
    int (*al)(wint_t, locale_t);
    int (*bl)(wint_t, locale_t);
    wint_t (*lo)(wint_t, locale_t);
    wint_t (*up)(wint_t, locale_t);

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    if (!nz(iswalnum_l(L'A', loc)) || nz(iswalnum_l(L' ', loc)))
        return 3;
    if (!nz(iswalpha_l(L'Z', loc)) || nz(iswalpha_l(L'1', loc)))
        return 4;
    if (!nz(iswcntrl_l(L'\n', loc)) || nz(iswcntrl_l(L'a', loc)))
        return 5;
    if (!nz(iswdigit_l(L'7', loc)) || nz(iswdigit_l(L'a', loc)))
        return 6;
    if (!nz(iswgraph_l(L'!', loc)) || nz(iswgraph_l(L' ', loc)))
        return 7;
    if (!nz(iswlower_l(L'a', loc)) || nz(iswlower_l(L'A', loc)))
        return 8;
    if (!nz(iswprint_l(L' ', loc)) || nz(iswprint_l(L'\n', loc)))
        return 9;
    if (!nz(iswpunct_l(L'.', loc)) || nz(iswpunct_l(L'a', loc)))
        return 10;
    if (!nz(iswspace_l(L' ', loc)) || nz(iswspace_l(L'a', loc)))
        return 11;
    if (!nz(iswupper_l(L'A', loc)) || nz(iswupper_l(L'a', loc)))
        return 12;
    if (!nz(iswxdigit_l(L'f', loc)) || nz(iswxdigit_l(L'g', loc)))
        return 13;
    if (!nz(iswblank_l(L' ', loc)) || !nz(iswblank_l(L'\t', loc))
        || nz(iswblank_l(L'\n', loc)))
        return 14;
    if (towlower_l(L'A', loc) != L'a' || towupper_l(L'b', loc) != L'B')
        return 15;
    if (iswalnum_l(WEOF, loc) || iswalpha_l(WEOF, loc) || iswcntrl_l(WEOF, loc)
        || iswdigit_l(WEOF, loc) || iswgraph_l(WEOF, loc) || iswlower_l(WEOF, loc)
        || iswprint_l(WEOF, loc) || iswpunct_l(WEOF, loc) || iswspace_l(WEOF, loc)
        || iswupper_l(WEOF, loc) || iswxdigit_l(WEOF, loc) || iswblank_l(WEOF, loc))
        return 16;
    if (towlower_l(WEOF, loc) != WEOF || towupper_l(WEOF, loc) != WEOF)
        return 17;
    if (!nz(iswalnum_l(L'A', LC_GLOBAL_LOCALE)) || nz(iswblank_l(L'\n', LC_GLOBAL_LOCALE)))
        return 18;
    if (towlower_l(L'Z', LC_GLOBAL_LOCALE) != L'z'
        || towupper_l(L'z', LC_GLOBAL_LOCALE) != L'Z')
        return 19;
    if (iswalnum_l(WEOF, LC_GLOBAL_LOCALE) || towlower_l(WEOF, LC_GLOBAL_LOCALE) != WEOF)
        return 20;
    al = iswalnum_l;
    bl = iswblank_l;
    lo = towlower_l;
    up = towupper_l;
    if (!nz(al(L'9', loc)) || nz(bl(L'\n', loc)))
        return 21;
    if (lo(L'Q', LC_GLOBAL_LOCALE) != L'q' || up(L'q', loc) != L'Q')
        return 22;
    freelocale(loc);
    return 0;
}
