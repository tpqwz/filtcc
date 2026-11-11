#include <sys/sysinfo.h>
int main(void)
{
    struct sysinfo a, b;
    int n;
    int (*fn)(struct sysinfo *);
    n = sysinfo(&a);
    if (n != 0 || a.uptime <= 0 || a.procs == 0)
        return 2;
    fn = sysinfo;
    n = fn(&b);
    if (n != 0 || b.procs == 0 || b.mem_unit == 0)
        return 3;
    return 0;
}
