#include <locale.h>
#include <string.h>
#include <time.h>

extern char *strptime_l(const char *, const char *, struct tm *, locale_t);

int main(void)
{
    struct tm tm;
    locale_t loc;
    char fmt[2];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    memset(&tm, 0, sizeof tm);
    fmt[0] = '%';
    fmt[1] = 'Y';
    (void)strptime_l("2020", fmt, &tm, loc);
    freelocale(loc);
    return 0;
}
