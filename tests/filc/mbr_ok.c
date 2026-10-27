#include <wchar.h>

int main(void)
{
    wchar_t wc;
    mbstate_t st = {0};
    char buf[8];
    char one[1];
    size_t n;
    size_t (*fn)(wchar_t *, const char *, size_t, mbstate_t *);

    wc = 99;
    n = mbrtowc(&wc, "a", 1, &st);
    if (n != 1 || wc != L'a')
        return 1;
    wc = 99;
    st = (mbstate_t){0};
    n = mbrtowc(&wc, "a", 0, &st);
    if (n != (size_t)-2 || wc != 99)
        return 2;
    st = (mbstate_t){0};
    if (mbrtowc(0, "a", 1, &st) != 1)
        return 3;
    wc = 99;
    st = (mbstate_t){0};
    if (mbrtowc(&wc, 0, 9, &st) != 0 || wc != 99)
        return 4;
    st = (mbstate_t){0};
    n = mbrtowc(&wc, "", 1, &st);
    if (n != 0 || wc != 0)
        return 5;

    one[0] = 'a';
    st = (mbstate_t){0};
    n = mbrtowc(&wc, one, 1, &st);
    if (n != 1 || wc != L'a')
        return 6;
    st = (mbstate_t){0};
    n = mbrtowc(&wc, "b", 1, 0);
    if (n != 1 || wc != L'b')
        return 7;

    st = (mbstate_t){0};
    n = wcrtomb(buf, L'a', &st);
    if (n != 1 || buf[0] != 'a')
        return 8;
    st = (mbstate_t){0};
    if (wcrtomb(0, L'Z', &st) != 1)
        return 9;

    st = (mbstate_t){0};
    if (mbrlen("ab", 1, &st) != 1 || mbrlen("ab", 1, 0) != 1)
        return 10;
    st = (mbstate_t){0};
    if (!mbsinit(0) || !mbsinit(&st))
        return 11;

    fn = mbrtowc;
    st = (mbstate_t){0};
    n = fn(&wc, "Z", 1, &st);
    if (n != 1 || wc != L'Z')
        return 12;
    return 0;
}
