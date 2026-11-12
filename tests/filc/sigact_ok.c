#include <signal.h>
#include <string.h>
static volatile int hits;
static void on_usr(int sig)
{
    (void)sig;
    hits++;
}
int main(void)
{
    struct sigaction act, old, cur;
    int n;
    int (*fn)(int, const struct sigaction *, struct sigaction *);
    memset(&act, 0, sizeof act);
    act.sa_handler = on_usr;
    sigemptyset(&act.sa_mask);
    n = sigaction(SIGUSR1, &act, &old);
    if (n != 0)
        return 2;
    if (raise(SIGUSR1) != 0 || hits != 1)
        return 3;
    n = sigaction(SIGUSR1, 0, &cur);
    if (n != 0 || cur.sa_handler != on_usr)
        return 4;
    n = sigaction(SIGUSR1, &cur, 0);
    if (n != 0)
        return 5;
    act.sa_handler = SIG_IGN;
    n = sigaction(SIGUSR1, &act, 0);
    if (n != 0)
        return 6;
    n = sigaction(SIGUSR1, &old, 0);
    if (n != 0)
        return 7;
    fn = sigaction;
    act.sa_handler = SIG_DFL;
    n = fn(SIGUSR1, &act, 0);
    if (n != 0)
        return 8;
    return 0;
}
