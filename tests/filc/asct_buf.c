#include <string.h>
#include <time.h>

int main(void)
{
    struct tm tm;
    char buf[25];

    memset(&tm, 0, sizeof tm);
    tm.tm_mday = 1;
    (void)asctime_r(&tm, buf);
    return 0;
}
