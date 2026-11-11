#include <string.h>
#include <time.h>

int main(void)
{
    struct tm tm;
    char buf[25];
    char *(*fa)(const struct tm *, char *);

    memset(&tm, 0, sizeof tm);
    tm.tm_mday = 1;
    fa = asctime_r;
    (void)fa(&tm, buf);
    return 0;
}
