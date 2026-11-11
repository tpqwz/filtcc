#include <string.h>
#include <time.h>
#include <wchar.h>

int main(void)
{
    struct tm tm;

    memset(&tm, 0, sizeof tm);
    tm.tm_year = 120;
    (void)wcsftime((wchar_t *)L"0123456789", 4, L"%Y", &tm);
    return 0;
}
