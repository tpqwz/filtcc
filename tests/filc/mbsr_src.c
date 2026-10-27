#include <string.h>
#include <wchar.h>

int main(void)
{
    char s[2];
    const char *p;
    wchar_t d[8];
    mbstate_t st;
    s[0] = 'a';
    s[1] = 'b';
    p = s;
    memset(&st, 0, sizeof st);
    mbsrtowcs(d, &p, 1, &st);
    return 0;
}
