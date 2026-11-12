#include <time.h>
int main(void)
{
    struct timespec ts, res;
    int n;
    int (*fn)(clockid_t, struct timespec *);
    n = clock_gettime(CLOCK_REALTIME, &ts);
    if (n != 0 || ts.tv_sec <= 0)
        return 2;
    n = clock_getres(CLOCK_REALTIME, &res);
    if (n != 0 || (res.tv_sec == 0 && res.tv_nsec == 0))
        return 3;
    n = clock_getres(CLOCK_REALTIME, 0);
    if (n != 0)
        return 4;
    fn = clock_gettime;
    n = fn(CLOCK_MONOTONIC, &ts);
    if (n != 0 || ts.tv_sec < 0)
        return 5;
    fn = clock_getres;
    n = fn(CLOCK_MONOTONIC, &res);
    if (n != 0)
        return 6;
    return 0;
}
