#include <locale.h>
#include <string.h>
#include <time.h>

int main(void)
{
    struct tm tm;
    locale_t loc;
    char buf[8];
    char fmt[2];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    memset(&tm, 0, sizeof tm);
    fmt[0] = '%';
    fmt[1] = 'Y';
    (void)strftime_l(buf, 8, fmt, &tm, loc);
    freelocale(loc);
    return 0;
}
