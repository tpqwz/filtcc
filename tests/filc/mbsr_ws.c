#include <string.h>
#include <wchar.h>

int main(void)
{
    wchar_t s[1];
    const wchar_t *p;
    char d[8];
    mbstate_t st;
    s[0] = L'a';
    p = s;
    memset(&st, 0, sizeof st);
    wcsrtombs(d, &p, 1, &st);
    return 0;
}
