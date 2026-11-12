#include <sched.h>
#include <sys/types.h>
int sched_getaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask);
int sched_setaffinity(pid_t pid, size_t cpusetsize, const cpu_set_t *mask);
static int on_cpu0(const cpu_set_t *set)
{
    const unsigned long *w;
    w = (const unsigned long *)set;
    return (w[0] & 1ul) != 0;
}
int main(void)
{
    cpu_set_t set;
    int n;
    int (*get)(pid_t, size_t, cpu_set_t *);
    int (*setfn)(pid_t, size_t, const cpu_set_t *);
    n = sched_getaffinity(0, sizeof set, &set);
    if (n != 0 || !on_cpu0(&set))
        return 2;
    n = sched_setaffinity(0, sizeof set, &set);
    if (n != 0)
        return 3;
    get = sched_getaffinity;
    n = get(0, sizeof set, &set);
    if (n != 0 || !on_cpu0(&set))
        return 4;
    setfn = sched_setaffinity;
    n = setfn(0, sizeof set, &set);
    if (n != 0)
        return 5;
    return 0;
}
