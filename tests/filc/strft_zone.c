#include <string.h>
#include <time.h>
int main(void)
{
    struct tm tm;
    char buf[16];
    char b[4] = {'a', 'b', 'c', 'd'};
    memset(&tm, 0, sizeof tm);
    tm.tm_year = 120;
    tm.tm_mday = 1;
    tm.tm_zone = b;
    (void)strftime(buf, sizeof buf, "%Z", &tm);
    return 0;
}
