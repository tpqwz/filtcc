#include <time.h>
int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *req,
    struct timespec *rem);
int main(void)
{
    struct timespec req;
    char b[4];
    req.tv_sec = 0;
    req.tv_nsec = 1000;
    (void)clock_nanosleep(CLOCK_MONOTONIC, 0, &req, (struct timespec *)b);
    return 0;
}
