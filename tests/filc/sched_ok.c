#include <sched.h>
#include <time.h>
int sched_getparam(pid_t pid, struct sched_param *param);
int sched_setparam(pid_t pid, const struct sched_param *param);
int sched_rr_get_interval(pid_t pid, struct timespec *tp);
int main(void)
{
    struct sched_param sp;
    struct timespec ts;
    int n;
    int (*get)(pid_t, struct sched_param *);
    int (*rr)(pid_t, struct timespec *);
    n = sched_getparam(0, &sp);
    if (n != 0)
        return 2;
    n = sched_setparam(0, &sp);
    if (n != 0)
        return 3;
    n = sched_rr_get_interval(0, &ts);
    if (n != 0 || ts.tv_nsec < 0)
        return 4;
    get = sched_getparam;
    rr = sched_rr_get_interval;
    n = get(0, &sp);
    if (n != 0)
        return 5;
    n = rr(0, &ts);
    if (n != 0)
        return 6;
    return 0;
}
