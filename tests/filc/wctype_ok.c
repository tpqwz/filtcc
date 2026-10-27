#include <wctype.h>

static int nz(int v)
{
    return v != 0;
}

int main(void)
{
    wctype_t a;
    wctype_t d;
    wctrans_t lo;
    wctrans_t up;
    wctype_t (*wt)(const char *);
    int (*ic)(wint_t, wctype_t);
    wctrans_t (*tr)(const char *);
    wint_t (*tc)(wint_t, wctrans_t);

    a = wctype("alpha");
    if (!a)
        return 2;
    if (!nz(iswctype(L'A', a)) || nz(iswctype(L'1', a)))
        return 3;
    d = wctype("digit");
    if (!d || !nz(iswctype(L'7', d)))
        return 4;
    if (wctype("nope") != 0)
        return 5;
    if (iswctype(WEOF, a) != 0)
        return 6;
    lo = wctrans("tolower");
    up = wctrans("toupper");
    if (!lo || !up)
        return 7;
    if (towctrans(L'A', lo) != L'a' || towctrans(L'b', up) != L'B')
        return 8;
    if (towctrans(WEOF, lo) != WEOF)
        return 9;
    wt = wctype;
    ic = iswctype;
    tr = wctrans;
    tc = towctrans;
    a = wt("upper");
    if (!a || !nz(ic(L'Q', a)) || nz(ic(L'q', a)))
        return 10;
    lo = tr("tolower");
    if (!lo || tc(L'Z', lo) != L'z')
        return 11;
    return 0;
}
