#include <string.h>
#include <sys/utsname.h>
int main(void)
{
    struct utsname u, v;
    int n;
    int (*fn)(struct utsname *);
    n = uname(&u);
    if (n != 0 || !memchr(u.sysname, 0, sizeof u.sysname) || u.sysname[0] == 0)
        return 2;
    if (!memchr(u.release, 0, sizeof u.release) || u.release[0] == 0)
        return 3;
    if (!memchr(u.machine, 0, sizeof u.machine) || u.machine[0] == 0)
        return 4;
    fn = uname;
    n = fn(&v);
    if (n != 0 || strcmp(u.sysname, v.sysname) != 0)
        return 5;
    return 0;
}
