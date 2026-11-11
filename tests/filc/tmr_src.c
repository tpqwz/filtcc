#include <string.h>
#include <time.h>

int main(void)
{
    struct tm tm;
    char tiny;

    memset(&tm, 0, sizeof tm);
    (void)gmtime_r((time_t *)&tiny, &tm);
    return 0;
}
