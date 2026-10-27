#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

int main(void)
{
    wchar_t wc;
    char buf[8];
    char one[1];
    int (*mb)(const char *, size_t);
    int (*mt)(wchar_t *, const char *, size_t);
    int n;

    if (mblen(0, 9) != 0)
        return 2;
    if (mblen("a", 1) != 1)
        return 3;
    if (mblen("", 1) != 0)
        return 4;
    if (mblen("\xff", 1) != -1)
        return 5;
    mblen(0, 0);
    one[0] = 'a';
    if (mblen(one, 0) != -1 || mblen(one, 1) != 1)
        return 6;

    wc = 99;
    if (mbtowc(&wc, 0, 9) != 0 || wc != 99)
        return 7;
    if (mbtowc(&wc, "a", 1) != 1 || wc != L'a')
        return 8;
    if (mbtowc(0, "Z", 1) != 1)
        return 9;
    wc = 99;
    if (mbtowc(&wc, "", 1) != 0 || wc != 0)
        return 10;
    wc = 99;
    if (mbtowc(&wc, "a", 0) != -1)
        return 11;

    if (wctomb(0, L'Z') != 0)
        return 12;
    buf[0] = 1;
    buf[1] = 2;
    if (wctomb(buf, L'a') != 1 || buf[0] != 'a' || buf[1] != 2)
        return 13;
    buf[0] = 9;
    buf[1] = 9;
    if (wctomb(buf, 0) != 1 || buf[0] != 0 || buf[1] != 9)
        return 14;

    mb = mblen;
    mt = mbtowc;
    if (mb("Q", 1) != 1)
        return 15;
    wc = 99;
    if (mt(&wc, "Q", 1) != 1 || wc != L'Q')
        return 16;

    if (!setlocale(LC_ALL, "C.UTF-8"))
        return 1;
    mblen(0, 0);
    if (mblen("\xc2\xa2", 2) != 2)
        return 17;
    mbtowc(0, 0, 0);
    wc = 99;
    n = mbtowc(&wc, "\xc2\xa2", 2);
    if (n != 2 || wc != 0xa2)
        return 18;
    mbtowc(0, 0, 0);
    wc = 99;
    if (mbtowc(&wc, "\xc2", 1) != -1 || wc != 99)
        return 19;
    n = wctomb(buf, 0xa2);
    if (n != 2 || (unsigned char)buf[0] != 0xc2
        || (unsigned char)buf[1] != 0xa2)
        return 20;
    if (wctomb(0, L'a') != 0)
        return 21;
    return 0;
}
