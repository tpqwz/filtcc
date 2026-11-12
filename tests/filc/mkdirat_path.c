#include <fcntl.h>
#include <sys/stat.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    (void)mkdirat(AT_FDCWD, p, 0700);
    return 0;
}
