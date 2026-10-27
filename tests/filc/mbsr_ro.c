#include <string.h>
#include <wchar.h>

static const char *const rop = "ab";

int main(void)
{
    wchar_t buf[8];
    mbstate_t st;
    memset(&st, 0, sizeof st);
    mbsrtowcs(buf, (const char **)&rop, 8, &st);
    return 0;
}
