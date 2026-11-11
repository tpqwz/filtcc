#include <time.h>

int main(void)
{
    time_t t = 0;
    char tiny;
    struct tm *(*fl)(const time_t *, struct tm *);

    fl = localtime_r;
    (void)fl(&t, (struct tm *)&tiny);
    return 0;
}
