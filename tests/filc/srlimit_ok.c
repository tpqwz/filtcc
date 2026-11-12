#include <sys/resource.h>
int main(void)
{
    struct rlimit r;
    int n;
    int (*fn)(int, const struct rlimit *);
    n = getrlimit(RLIMIT_NOFILE, &r);
    if (n != 0 || r.rlim_cur == 0)
        return 2;
    n = setrlimit(RLIMIT_NOFILE, &r);
    if (n != 0)
        return 3;
    fn = setrlimit;
    n = fn(RLIMIT_NOFILE, &r);
    if (n != 0)
        return 4;
    return 0;
}
