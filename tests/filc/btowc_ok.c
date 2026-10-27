#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main(void)
{
    wint_t (*b)(int);
    int (*w)(wint_t);

    if (btowc('a') != L'a')
        return 2;
    if (btowc(0) != 0)
        return 3;
    if (btowc(EOF) != WEOF)
        return 4;
    if (btowc(255) != WEOF)
        return 5;
    if (wctob(L'a') != 'a')
        return 6;
    if (wctob(0) != 0)
        return 7;
    if (wctob(WEOF) != EOF)
        return 8;
    if (wctob(255) != EOF)
        return 9;
    b = btowc;
    w = wctob;
    if (b('Z') != L'Z' || w(L'Z') != 'Z')
        return 10;
    if (!setlocale(LC_ALL, "C.UTF-8"))
        return 1;
    if (btowc(255) != WEOF || wctob(0x20ac) != EOF)
        return 11;
    if (btowc('a') != L'a' || wctob(L'a') != 'a')
        return 12;
    return 0;
}
