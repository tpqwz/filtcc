#include <sys/time.h>
int main(void)
{
    struct itimerval neu;
    char b[4];
    neu.it_interval.tv_sec = 0;
    neu.it_interval.tv_usec = 0;
    neu.it_value.tv_sec = 0;
    neu.it_value.tv_usec = 0;
    (void)setitimer(ITIMER_REAL, &neu, (struct itimerval *)b);
    return 0;
}
