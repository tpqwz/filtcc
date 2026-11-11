#include <string.h>
#include <time.h>
#include <wchar.h>

int main(void)
{
    struct tm tm;
    wchar_t tiny[1];

    memset(&tm, 0, sizeof tm);
    tm.tm_year = 120;
    tiny[0] = 0;
    (void)wcsftime(tiny, 4, L"%Y", &tm);
    return 0;
}
