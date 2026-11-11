#include <string.h>
#include <time.h>

int main(void)
{
    struct tm tm;
    char buf[8];
    char fmt[2];
    size_t (*fn)(char *, size_t, const char *, const struct tm *);

    memset(&tm, 0, sizeof tm);
    fmt[0] = '%';
    fmt[1] = 'Y';
    fn = strftime;
    (void)fn(buf, 8, fmt, &tm);
    return 0;
}
