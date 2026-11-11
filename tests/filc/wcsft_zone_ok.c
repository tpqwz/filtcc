#include <string.h>
#include <time.h>
#include <wchar.h>
int main(void)
{
    struct tm tm;
    wchar_t buf[16];
    char z[] = "UTC";
    char b[4] = {'a', 'b', 'c', 'd'};
    size_t n;
    size_t (*fn)(wchar_t *, size_t, const wchar_t *, const struct tm *);

    memset(&tm, 0, sizeof tm);
    tm.tm_year = 120;
    tm.tm_mon = 0;
    tm.tm_mday = 2;
    tm.tm_zone = z;
    n = wcsftime(buf, 16, L"%Z", &tm);
    if (n != 3 || wcscmp(buf, L"UTC") != 0)
        return 2;
    tm.tm_zone = b;
    n = wcsftime(buf, 16, L"%Y", &tm);
    if (n != 4 || wcscmp(buf, L"2020") != 0)
        return 3;
    n = wcsftime(buf, 16, L"%%Z", &tm);
    if (n != 2 || wcscmp(buf, L"%Z") != 0)
        return 4;
    tm.tm_zone = z;
    fn = wcsftime;
    n = fn(buf, 16, L"%Z", &tm);
    if (n != 3 || wcscmp(buf, L"UTC") != 0)
        return 5;
    return 0;
}
