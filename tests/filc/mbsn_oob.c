#include <wchar.h>

int main(void)
{
    char raw[2];
    const char *p;
    wchar_t d[4];
    mbstate_t st = {0};
    raw[0] = 'a';
    raw[1] = 'b';
    p = raw;
    (void)mbsnrtowcs(d, &p, 3, 4, &st);
    return 0;
}
