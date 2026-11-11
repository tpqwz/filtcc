#include <string.h>
#include <time.h>

extern char *strptime(const char *, const char *, struct tm *);

int main(void)
{
    struct tm tm;
    char fmt[2];
    char *(*fn)(const char *, const char *, struct tm *);

    memset(&tm, 0, sizeof tm);
    fmt[0] = '%';
    fmt[1] = 'Y';
    fn = strptime;
    (void)fn("2020", fmt, &tm);
    return 0;
}
