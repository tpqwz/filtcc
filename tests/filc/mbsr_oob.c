#include <string.h>
#include <wchar.h>

int main(void)
{
    wchar_t d[1];
    const char *p = "abcd";
    mbstate_t st;
    memset(&st, 0, sizeof st);
    mbsrtowcs(d, &p, 4, &st);
    return 0;
}
