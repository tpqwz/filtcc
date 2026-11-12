#include <sched.h>
int sched_getparam(pid_t pid, struct sched_param *param);
int main(void)
{
    char b[1];
    (void)sched_getparam(0, (struct sched_param *)b);
    return 0;
}
