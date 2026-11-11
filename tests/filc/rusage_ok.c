#include <sys/resource.h>
int main(void)
{
    struct rusage a, b;
    int n;
    int (*fn)(int, struct rusage *);
    n = getrusage(RUSAGE_SELF, &a);
    if (n != 0 || a.ru_maxrss <= 0)
        return 2;
    fn = getrusage;
    n = fn(RUSAGE_SELF, &b);
    if (n != 0 || b.ru_maxrss <= 0)
        return 3;
    n = getrusage(RUSAGE_CHILDREN, &a);
    if (n != 0)
        return 4;
    return 0;
}
