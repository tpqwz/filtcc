#include <time.h>

int main(void)
{
    time_t t = 0;
    char tiny;

    (void)gmtime_r(&t, (struct tm *)&tiny);
    return 0;
}
