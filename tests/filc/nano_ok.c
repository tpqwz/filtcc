#include <time.h>
int main(void)
{
    struct timespec req, rem;
    int n;
    int (*fn)(const struct timespec *, struct timespec *);
    req.tv_sec = 0;
    req.tv_nsec = 1000;
    n = nanosleep(&req, &rem);
    if (n != 0)
        return 2;
    n = nanosleep(&req, 0);
    if (n != 0)
        return 3;
    fn = nanosleep;
    n = fn(&req, 0);
    if (n != 0)
        return 4;
    return 0;
}
