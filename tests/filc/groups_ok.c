#include <unistd.h>
int main(void)
{
    gid_t list[64];
    gid_t mark;
    int n, m;
    int (*fn)(int, gid_t *);
    n = getgroups(0, 0);
    if (n < 1 || n > 64)
        return 2;
    m = getgroups(n, list);
    if (m != n)
        return 3;
    mark = (gid_t)0x5a5a5a5a;
    m = getgroups(0, &mark);
    if (m != n || mark != (gid_t)0x5a5a5a5a)
        return 4;
    fn = getgroups;
    m = fn(n, list);
    if (m != n)
        return 5;
    return 0;
}
