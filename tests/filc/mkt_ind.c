#include <time.h>

int main(void)
{
    char tiny;
    time_t (*fm)(struct tm *);

    fm = mktime;
    (void)fm((struct tm *)&tiny);
    return 0;
}
