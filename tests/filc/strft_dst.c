#include <string.h>
#include <time.h>

int main(void)
{
    struct tm tm;
    char tiny[1];

    memset(&tm, 0, sizeof tm);
    tm.tm_year = 120;
    tiny[0] = 0;
    (void)strftime(tiny, 4, "%Y", &tm);
    return 0;
}
