#include <wchar.h>

static const char *const rop = "ab";

int main(void)
{
    wchar_t d[4];
    mbstate_t st = {0};
    (void)mbsnrtowcs(d, (const char **)&rop, 1, 4, &st);
    return 0;
}
