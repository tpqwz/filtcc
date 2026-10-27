#include <wchar.h>

int main(void)
{
    size_t (*fn)(wchar_t *, const char **, size_t, size_t, mbstate_t *);
    char raw[2];
    const char *p;
    wchar_t d[4];
    mbstate_t st = {0};
    raw[0] = 'a';
    raw[1] = 'b';
    p = raw;
    fn = mbsnrtowcs;
    (void)fn(d, &p, 3, 4, &st);
    return 0;
}
