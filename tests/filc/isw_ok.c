#include <wchar.h>
#include <wctype.h>

static int nz(int v)
{
    return v != 0;
}

int main(void)
{
    int (*al)(wint_t);
    int (*aa)(wint_t);
    int (*cn)(wint_t);
    int (*di)(wint_t);
    int (*gr)(wint_t);
    int (*lo)(wint_t);
    int (*pr)(wint_t);
    int (*pu)(wint_t);
    int (*sp)(wint_t);
    int (*up)(wint_t);
    int (*xd)(wint_t);
    int (*bl)(wint_t);

    if (!nz(iswalnum(L'A')) || nz(iswalnum(L' ')))
        return 2;
    if (!nz(iswalpha(L'Z')) || nz(iswalpha(L'1')))
        return 3;
    if (!nz(iswcntrl(L'\n')) || nz(iswcntrl(L'a')))
        return 4;
    if (!nz(iswdigit(L'7')) || nz(iswdigit(L'a')))
        return 5;
    if (!nz(iswgraph(L'!')) || nz(iswgraph(L' ')))
        return 6;
    if (!nz(iswlower(L'a')) || nz(iswlower(L'A')))
        return 7;
    if (!nz(iswprint(L' ')) || nz(iswprint(L'\n')))
        return 8;
    if (!nz(iswpunct(L'.')) || nz(iswpunct(L'a')))
        return 9;
    if (!nz(iswspace(L' ')) || nz(iswspace(L'a')))
        return 10;
    if (!nz(iswupper(L'A')) || nz(iswupper(L'a')))
        return 11;
    if (!nz(iswxdigit(L'f')) || nz(iswxdigit(L'g')))
        return 12;
    if (!nz(iswblank(L' ')) || !nz(iswblank(L'\t')) || nz(iswblank(L'\n')))
        return 13;
    if (iswalnum(WEOF) || iswalpha(WEOF) || iswcntrl(WEOF) || iswdigit(WEOF)
        || iswgraph(WEOF) || iswlower(WEOF) || iswprint(WEOF) || iswpunct(WEOF)
        || iswspace(WEOF) || iswupper(WEOF) || iswxdigit(WEOF) || iswblank(WEOF))
        return 14;
    al = iswalnum;
    aa = iswalpha;
    cn = iswcntrl;
    di = iswdigit;
    gr = iswgraph;
    lo = iswlower;
    pr = iswprint;
    pu = iswpunct;
    sp = iswspace;
    up = iswupper;
    xd = iswxdigit;
    bl = iswblank;
    if (!nz(al(L'9')) || nz(aa(L'9')) || !nz(cn(L'\t')) || !nz(di(L'0')))
        return 15;
    if (!nz(gr(L'A')) || !nz(lo(L'z')) || !nz(pr(L'A')) || !nz(pu(L',')))
        return 16;
    if (!nz(sp(L'\n')) || !nz(up(L'Q')) || !nz(xd(L'A')) || nz(bl(L'\n')))
        return 17;
    return 0;
}
