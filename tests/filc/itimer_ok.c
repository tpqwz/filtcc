#include <sys/time.h>
int main(void)
{
    struct itimerval cur, neu, old;
    int n;
    int (*get)(int, struct itimerval *);
    int (*set)(int, const struct itimerval *, struct itimerval *);
    n = getitimer(ITIMER_REAL, &cur);
    if (n != 0)
        return 2;
    neu = cur;
    n = setitimer(ITIMER_REAL, &neu, &old);
    if (n != 0)
        return 3;
    n = setitimer(ITIMER_REAL, &neu, 0);
    if (n != 0)
        return 4;
    get = getitimer;
    n = get(ITIMER_REAL, &cur);
    if (n != 0)
        return 5;
    set = setitimer;
    n = set(ITIMER_REAL, &neu, 0);
    if (n != 0)
        return 6;
    return 0;
}
