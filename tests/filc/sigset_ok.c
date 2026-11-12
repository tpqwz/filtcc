#include <signal.h>
int main(void)
{
    sigset_t s;
    int n;
    int (*empty)(sigset_t *);
    int (*add)(sigset_t *, int);
    int (*mem)(const sigset_t *, int);
    n = sigemptyset(&s);
    if (n != 0)
        return 2;
    n = sigaddset(&s, SIGINT);
    if (n != 0 || sigismember(&s, SIGINT) != 1)
        return 3;
    n = sigdelset(&s, SIGINT);
    if (n != 0 || sigismember(&s, SIGINT) != 0)
        return 4;
    n = sigfillset(&s);
    if (n != 0 || sigismember(&s, SIGINT) != 1)
        return 5;
    empty = sigemptyset;
    add = sigaddset;
    mem = sigismember;
    n = empty(&s);
    if (n != 0 || mem(&s, SIGINT) != 0)
        return 6;
    n = add(&s, SIGTERM);
    if (n != 0 || mem(&s, SIGTERM) != 1)
        return 7;
    return 0;
}
