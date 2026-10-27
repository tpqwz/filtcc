#include <stdint.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
    wchar_t buf[8];
    mbstate_t st;
    const char *p = "hi";
    uintptr_t bits = (uintptr_t)p;
    memset(&st, 0, sizeof st);
    if (mbsrtowcs(buf, &p, 8, &st) != 2 || p != NULL)
        return 1;
    *(uintptr_t *)&p = bits;
    strlen(p);
    return 0;
}
