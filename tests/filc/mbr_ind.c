#include <wchar.h>

int main(void)
{
    size_t (*fn)(wchar_t *, const char *, size_t, mbstate_t *);
    wchar_t wc;
    mbstate_t st = {0};
    char src[1];
    src[0] = 'a';
    fn = mbrtowc;
    (void)fn(&wc, src, 2, &st);
    return 0;
}
