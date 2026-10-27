#include <wchar.h>

int main(void)
{
    char tiny[1];
    const char *p = "ab";
    wchar_t d[4];
    (void)mbsnrtowcs(d, &p, 1, 4, (mbstate_t *)tiny);
    return 0;
}
