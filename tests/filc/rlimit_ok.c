#include <sys/resource.h>
int main(void)
{
    struct rlimit a, b;
    int n;
    int (*fn)(int, struct rlimit *);
    n = getrlimit(RLIMIT_NOFILE, &a);
    if (n != 0 || a.rlim_cur == 0)
        return 2;
    fn = getrlimit;
    n = fn(RLIMIT_NOFILE, &b);
    if (n != 0 || b.rlim_cur != a.rlim_cur)
        return 3;
    n = getrlimit(RLIMIT_STACK, &a);
    if (n != 0 || a.rlim_cur == 0)
        return 4;
    return 0;
}
