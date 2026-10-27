#include <string.h>
#include <wchar.h>

int main(void)
{
    size_t (*fn)(wchar_t *, const char **, size_t, mbstate_t *) = mbsrtowcs;
    wchar_t d[1];
    const char *p = "abcd";
    mbstate_t st;
    memset(&st, 0, sizeof st);
    fn(d, &p, 4, &st);
    return 0;
}
