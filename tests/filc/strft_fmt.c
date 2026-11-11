#include <string.h>
#include <time.h>

int main(void)
{
    struct tm tm;
    char buf[8];
    char fmt[2];

    memset(&tm, 0, sizeof tm);
    fmt[0] = '%';
    fmt[1] = 'Y';
    (void)strftime(buf, 8, fmt, &tm);
    return 0;
}
