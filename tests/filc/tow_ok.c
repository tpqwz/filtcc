#include <wchar.h>
#include <wctype.h>

int main(void)
{
    wint_t (*lo)(wint_t);
    wint_t (*up)(wint_t);

    if (towlower(L'A') != L'a' || towlower(L'a') != L'a')
        return 2;
    if (towupper(L'b') != L'B' || towupper(L'B') != L'B')
        return 3;
    if (towlower(WEOF) != WEOF || towupper(WEOF) != WEOF)
        return 4;
    lo = towlower;
    up = towupper;
    if (lo(L'Z') != L'z')
        return 5;
    if (up(L'z') != L'Z')
        return 6;
    return 0;
}
