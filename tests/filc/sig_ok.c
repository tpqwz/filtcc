#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
static volatile int got;
static void on_alarm(int sig)
{
    (void)sig;
    got = 1;
}
int main(void)
{
    int i;
    signal(SIGALRM, on_alarm);
    alarm(1);
    for (i = 0; i < 10000000 && !got; i++) {
        char *p = malloc(16);
        if (!p)
            return 1;
        p[0] = 1;
        free(p);
    }
    if (!got)
        return 2;
    return 0;
}
