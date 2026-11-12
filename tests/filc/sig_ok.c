#include <signal.h>
int main(void)
{
    sigset_t cur, old;
    int n;
    int (*pend)(sigset_t *);
    int (*mask)(int, const sigset_t *, sigset_t *);
    n = sigpending(&cur);
    if (n != 0)
        return 2;
    n = sigprocmask(SIG_BLOCK, 0, &old);
    if (n != 0)
        return 3;
    n = sigprocmask(SIG_SETMASK, &old, 0);
    if (n != 0)
        return 4;
    n = sigprocmask(SIG_BLOCK, 0, 0);
    if (n != 0)
        return 5;
    pend = sigpending;
    n = pend(&cur);
    if (n != 0)
        return 6;
    mask = sigprocmask;
    n = mask(SIG_BLOCK, 0, &old);
    if (n != 0)
        return 7;
    return 0;
}
