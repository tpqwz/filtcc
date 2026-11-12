#include <sched.h>
#include <sys/types.h>
int sched_getaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask);
int main(void)
{
    char b[8];
    (void)sched_getaffinity(0, 128, (cpu_set_t *)b);
    return 0;
}
