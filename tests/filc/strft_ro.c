#include <string.h>
#include <time.h>

int main(void)
{
    struct tm tm;

    memset(&tm, 0, sizeof tm);
    tm.tm_year = 120;
    (void)strftime((char *)"0123456789", 4, "%Y", &tm);
    return 0;
}
