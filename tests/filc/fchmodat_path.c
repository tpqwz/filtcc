#include <fcntl.h>
#include <sys/stat.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    (void)fchmodat(AT_FDCWD, p, 0700, 0);
    return 0;
}
