#include <sys/time.h>
int main(void)
{
    char b[4];
    (void)getitimer(ITIMER_REAL, (struct itimerval *)b);
    return 0;
}
