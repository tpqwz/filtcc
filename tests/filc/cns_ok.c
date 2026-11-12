#include <time.h>
int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *req,
    struct timespec *rem);
int main(void)
{
    struct timespec req, rem;
    int n;
    int (*fn)(clockid_t, int, const struct timespec *, struct timespec *);
    req.tv_sec = 0;
    req.tv_nsec = 1000;
    n = clock_nanosleep(CLOCK_MONOTONIC, 0, &req, &rem);
    if (n != 0)
        return 2;
    n = clock_nanosleep(CLOCK_MONOTONIC, 0, &req, 0);
    if (n != 0)
        return 3;
    fn = clock_nanosleep;
    n = fn(CLOCK_MONOTONIC, 0, &req, 0);
    if (n != 0)
        return 4;
    return 0;
}
