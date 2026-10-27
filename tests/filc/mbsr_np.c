#include <string.h>
#include <wchar.h>

int main(void)
{
    const char *p = NULL;
    wchar_t d[4];
    mbstate_t st;
    memset(&st, 0, sizeof st);
    mbsrtowcs(d, &p, 4, &st);
    return 0;
}
