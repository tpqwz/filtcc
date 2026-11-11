#include <string.h>
#include <time.h>

extern char *strptime(const char *, const char *, struct tm *);

int main(void)
{
    struct tm tm;
    char *p;

    memset(&tm, 0, sizeof tm);
    p = strptime("2020-01-02", "%Y-%m-%d", &tm);
    *p = 'x';
    return 0;
}
