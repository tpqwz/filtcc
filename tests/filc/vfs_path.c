#include <sys/statvfs.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    struct statvfs v;
    (void)statvfs(p, &v);
    return 0;
}
