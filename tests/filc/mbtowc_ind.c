#include <stdlib.h>

int main(void)
{
    int (*fn)(wchar_t *, const char *, size_t);
    wchar_t wc;
    char src[1];
    src[0] = 'a';
    fn = mbtowc;
    (void)fn(&wc, src, 2);
    return 0;
}
