#include <string.h>
#include <time.h>
#include <wchar.h>
int main(void)
{
    struct tm tm;
    wchar_t buf[16];
    char b[4] = {'a', 'b', 'c', 'd'};
    memset(&tm, 0, sizeof tm);
    tm.tm_year = 120;
    tm.tm_mday = 1;
    tm.tm_zone = b;
    (void)wcsftime(buf, 16, L"%Z", &tm);
    return 0;
}
