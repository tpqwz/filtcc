#include <string.h>
#include <time.h>
#include <wchar.h>

int main(void)
{
    struct tm tm;
    wchar_t buf[8];
    wchar_t fmt[2];
    size_t (*fn)(wchar_t *, size_t, const wchar_t *, const struct tm *);

    memset(&tm, 0, sizeof tm);
    fmt[0] = L'%';
    fmt[1] = L'Y';
    fn = wcsftime;
    (void)fn(buf, 8, fmt, &tm);
    return 0;
}
