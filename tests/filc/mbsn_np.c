#include <wchar.h>

int main(void)
{
    const char *p = 0;
    wchar_t d[4];
    mbstate_t st = {0};
    (void)mbsnrtowcs(d, &p, 1, 4, &st);
    return 0;
}
