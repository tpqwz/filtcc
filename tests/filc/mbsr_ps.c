#include <wchar.h>

int main(void)
{
    wchar_t buf[8];
    const char *p = "ab";
    char tiny[1];
    mbsrtowcs(buf, &p, 8, (mbstate_t *)tiny);
    return 0;
}
