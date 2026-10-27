#include <wchar.h>

int main(void)
{
    wchar_t d[1];
    const char *p = "ab";
    mbstate_t st = {0};
    (void)mbsnrtowcs(d, &p, 2, 4, &st);
    return 0;
}
