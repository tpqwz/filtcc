#include <locale.h>
#include <wctype.h>

static int nz(int v)
{
    return v != 0;
}

int main(void)
{
    locale_t loc;
    wctype_t a;
    wctype_t d;
    wctrans_t lo;
    wctrans_t up;
    wctype_t (*wt)(const char *, locale_t);
    int (*ic)(wint_t, wctype_t, locale_t);
    wctrans_t (*tr)(const char *, locale_t);
    wint_t (*tc)(wint_t, wctrans_t, locale_t);

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    a = wctype_l("alpha", loc);
    if (!a)
        return 3;
    if (!nz(iswctype_l(L'A', a, loc)) || nz(iswctype_l(L'1', a, loc)))
        return 4;
    d = wctype_l("digit", loc);
    if (!d || !nz(iswctype_l(L'7', d, loc)))
        return 5;
    if (wctype_l("nope", loc) != 0)
        return 6;
    if (iswctype_l(WEOF, a, loc) != 0)
        return 7;
    lo = wctrans_l("tolower", loc);
    up = wctrans_l("toupper", loc);
    if (!lo || !up)
        return 8;
    if (towctrans_l(L'A', lo, loc) != L'a' || towctrans_l(L'b', up, loc) != L'B')
        return 9;
    if (towctrans_l(WEOF, lo, loc) != WEOF)
        return 10;
    a = wctype_l("alpha", LC_GLOBAL_LOCALE);
    if (!a || !nz(iswctype_l(L'A', a, LC_GLOBAL_LOCALE)))
        return 11;
    if (wctype_l("nope", LC_GLOBAL_LOCALE) != 0)
        return 12;
    lo = wctrans_l("tolower", LC_GLOBAL_LOCALE);
    if (!lo || towctrans_l(L'Z', lo, LC_GLOBAL_LOCALE) != L'z')
        return 13;
    if (towctrans_l(WEOF, lo, LC_GLOBAL_LOCALE) != WEOF)
        return 14;
    wt = wctype_l;
    ic = iswctype_l;
    tr = wctrans_l;
    tc = towctrans_l;
    a = wt("upper", loc);
    if (!a || !nz(ic(L'Q', a, loc)) || nz(ic(L'q', a, loc)))
        return 15;
    if (!nz(ic(L'Q', a, LC_GLOBAL_LOCALE)))
        return 16;
    lo = tr("tolower", LC_GLOBAL_LOCALE);
    if (!lo || tc(L'Z', lo, loc) != L'z')
        return 17;
    if (tc(L'Z', lo, LC_GLOBAL_LOCALE) != L'z')
        return 18;
    freelocale(loc);
    return 0;
}
