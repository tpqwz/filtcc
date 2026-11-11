#include <string.h>
#include <time.h>

int main(void)
{
    struct tm tm;

    memset(&tm, 0, sizeof tm);
    (void)gmtime_r(0, &tm);
    return 0;
}
