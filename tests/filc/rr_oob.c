#include <sched.h>
#include <time.h>
int sched_rr_get_interval(pid_t pid, struct timespec *tp);
int main(void)
{
    char b[4];
    (void)sched_rr_get_interval(0, (struct timespec *)b);
    return 0;
}
