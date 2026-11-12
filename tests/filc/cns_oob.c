#include <time.h>
int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *req,
    struct timespec *rem);
int main(void)
{
    char b[4];
    (void)clock_nanosleep(CLOCK_MONOTONIC, 0, (struct timespec *)b, 0);
    return 0;
}
