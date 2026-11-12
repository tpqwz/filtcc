#include <sched.h>
int sched_setparam(pid_t pid, const struct sched_param *param);
int main(void)
{
    char b[1];
    (void)sched_setparam(0, (struct sched_param *)b);
    return 0;
}
