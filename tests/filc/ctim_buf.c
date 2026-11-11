#include <time.h>

int main(void)
{
    time_t t = 0;
    char buf[25];

    (void)ctime_r(&t, buf);
    return 0;
}
