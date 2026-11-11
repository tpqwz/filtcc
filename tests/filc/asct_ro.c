#include <string.h>
#include <time.h>

int main(void)
{
    struct tm tm;

    memset(&tm, 0, sizeof tm);
    tm.tm_mday = 1;
    (void)asctime_r(&tm, (char *)"0123456789");
    return 0;
}
