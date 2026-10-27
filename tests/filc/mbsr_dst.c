#include <string.h>
#include <wchar.h>

int main(void)
{
    char d[1];
    const wchar_t *p = L"abcd";
    mbstate_t st;
    memset(&st, 0, sizeof st);
    wcsrtombs(d, &p, 4, &st);
    return 0;
}
